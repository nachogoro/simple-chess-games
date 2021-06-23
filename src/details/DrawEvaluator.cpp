#include "DrawEvaluator.h"

#include <details/BoardAnalyzer.h>
#include <details/MoveValidator.h>
#include <details/fen/FenUtils.h>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	bool enoughMatingMaterial(const Board& board)
	{
		// Only the following combinations are considered insufficient
		// material:
		//   - King vs King
		//   - King + Bishop vs King
		//   - King + Knight vs King
		//   - King + Bishop vs King + Bishop (same coloured bishops)
		std::set<Piece> whitePieces;
		std::set<Piece> blackPieces;

		const std::map<Square, Piece> occupiedSquares = board.occupiedSquares();

		for (const auto& entry : occupiedSquares)
		{
			const Piece& piece = entry.second;

			if (piece.color() == COLOR_WHITE)
			{
				whitePieces.insert(piece);
			}
			else
			{
				blackPieces.insert(piece);
			}
		}

		if (whitePieces.size() == 1 && blackPieces.size() == 1)
		{
			// King vs king
			return false;
		}

		if (whitePieces.size() > 2 || blackPieces.size() > 2)
		{
			// At least one side has more than 2 pieces, this is always enough
			// to theoretically mate.
			return true;
		}

		if (whitePieces.size() == 2 && blackPieces.size() == 2)
		{
			if (!(whitePieces.count({TYPE_BISHOP, COLOR_WHITE})
					&& blackPieces.count({TYPE_BISHOP, COLOR_BLACK})))
			{
				// If both sides do not have King + Bishop but have two pieces,
				// mate is theoretically possible
				return true;
			}

			// Both sides have King + Bishop, we need to figure out if they are
			// of the same color
			boost::optional<Color> bishopColor;
			for (const auto& entry : occupiedSquares)
			{
				const Square& sq = entry.first;
				const Piece& piece = entry.second;

				if (piece.type() == TYPE_BISHOP)
				{
					if (!bishopColor)
					{
						// First bishop we encounter
						bishopColor = {sq.color()};
					}
					else
					{
						if (sq.color() != *bishopColor)
						{
							// Bishop of different colors, mate is possible
							return true;
						}
						else
						{
							// Same-colored bishops, no mate is possible
							return false;
						}
					}
				}
			}

			// Should be unreachable
			throw std::runtime_error(
					"Inconsistency when evaluating whether material "
					"is sufficient (bishops)");
		}

		// One side has only the King and the other has King + some other piece
		const std::set<Piece>& piecesOfRelevantSide
			= (whitePieces.size() > 1)
				? whitePieces
				: blackPieces;

		const std::set<PieceType> drawingTypes
			= {TYPE_KING, TYPE_KNIGHT, TYPE_BISHOP};

		for (const Piece& piece : piecesOfRelevantSide)
		{
			if (drawingTypes.count(piece.type()) == 0)
			{
				// Found one piece which is neither King, Bishop or Knight
				return true;
			}
		}

		return false;
	}

}

boost::optional<DrawReason> DrawEvaluator::reasonToDraw(
		const GameStage& stage)
{
	return reasonToDraw(stage, {});
}

boost::optional<DrawReason> DrawEvaluator::reasonToDraw(
		const GameStage& stage,
		const std::map<std::string, uint8_t>& previouslyReachedPositions)
{
	if (stage.halfMovesSinceLastCaptureOrPawnAdvance() >= 150)
	{
		return { DRAW_REASON_75_MOVE_RULE };
	}

	// It is possible the current stage if the fifth repetition
	const std::string relevantFen
		= FenUtils::fenForRepetitions(stage.fen());

	const uint8_t timesPositionAppearedPreviously =
		(previouslyReachedPositions.count(relevantFen) != 0)
			? previouslyReachedPositions.at(relevantFen)
			: 0;

	if (timesPositionAppearedPreviously >= 4)
	{
		return { DRAW_REASON_FIVE_FOLD_REPETITION };
	}

	const std::set<PieceMove> allPossibleMoves
		= MoveValidator::allAvailableMoves(
				stage.board(),
				stage.move()
					? MoveValidator::enPassantTarget(stage.move()->pieceMove())
					: boost::optional<Square>{},
				stage.castlingRights(),
				stage.activeColor());

	if (allPossibleMoves.size() == 0
			&& !BoardAnalyzer::isInCheck(stage.board(), stage.activeColor()))
	{
		return { DRAW_REASON_STALEMATE };
	}

	if (!internal::enoughMatingMaterial(stage.board()))
	{
		return { DRAW_REASON_INSUFFICIENT_MATERIAL };
	}

	if (stage.move() && stage.move()->isDrawOffered())
	{
		return { DRAW_REASON_OFFERED_AND_ACCEPTED };
	}

	if (stage.halfMovesSinceLastCaptureOrPawnAdvance() >= 100)
	{
		return { DRAW_REASON_50_MOVE_RULE };
	}

	// A player can claim a draw if the current position has been reached at
	// least twice before times, or if the position emerging from their move in
	// their current turn has appeared at least twice before. Hence, we need to
	// figure out if any of the positions which might emerge from this move
	// would cause 3-fold repetition
	if (timesPositionAppearedPreviously >= 2)
	{
		return { DRAW_REASON_THREE_FOLD_REPETITION };
	}

	for (const auto& move : allPossibleMoves)
	{
		// To achieve the hypothetical next stage we don't care about draw
		// offers
		const GameStage nextStage
			= stage.makeMove(move, false);

		const std::string relevantFen
			= FenUtils::fenForRepetitions(nextStage.fen());

		if (previouslyReachedPositions.count(relevantFen)
				&& previouslyReachedPositions.at(relevantFen) >= 2)
		{
			return { DRAW_REASON_THREE_FOLD_REPETITION };
		}
	}

	return {};
}
