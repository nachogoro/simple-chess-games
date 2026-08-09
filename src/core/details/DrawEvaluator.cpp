#include "DrawEvaluator.h"

#include "BoardAccess.h"
#include "BoardAnalyzer.h"
#include "MoveValidator.h"
#include "fen/FenUtils.h"

#include <algorithm>
#include <array>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	bool opponentHasOnlyKing(const Board& board, Color activeColor)
	{
		const Color opponentColor
			= (activeColor == Color::White) ? Color::Black : Color::White;

		const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

		for (uint8_t index = 0; index < 64; ++index)
		{
			const uint8_t code = squares[index];

			if (BoardAccess::isColor(code, opponentColor)
					&& BoardAccess::typeOf(code) != PieceType::King)
			{
				return false;
			}
		}

		return true;
	}

	bool enoughMatingMaterial(const Board& board)
	{
		// Only the following combinations are considered insufficient
		// material:
		//   - King vs King
		//   - King + Bishop vs King
		//   - King + Knight vs King
		//   - King + Bishop vs King + Bishop (same coloured bishops)

		// Which piece types each side has on the board, one bit per type.
		// Only the distinct types matter here, never how many of each, so a
		// bit mask per side answers every question below without allocating
		// anything.
		uint8_t typesPresent[2] = {0, 0};

		const auto typeCount = [](const uint8_t mask) {
			uint8_t count = 0;
			for (uint8_t bit = 0; bit < 8; ++bit)
			{
				count = static_cast<uint8_t>(count + ((mask >> bit) & 1));
			}
			return count;
		};

		const auto has = [](const uint8_t mask, const PieceType type) {
			return (mask & (1 << static_cast<uint8_t>(type))) != 0;
		};

		const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

		for (uint8_t index = 0; index < 64; ++index)
		{
			const uint8_t code = squares[index];

			if (code == 0)
			{
				continue;
			}

			const size_t side
				= BoardAccess::isColor(code, Color::White) ? 0 : 1;

			typesPresent[side] = static_cast<uint8_t>(
					typesPresent[side]
					| (1 << static_cast<uint8_t>(BoardAccess::typeOf(code))));
		}

		const uint8_t whiteTypes = typeCount(typesPresent[0]);
		const uint8_t blackTypes = typeCount(typesPresent[1]);

		if (whiteTypes == 1 && blackTypes == 1)
		{
			// King vs king
			return false;
		}

		if (whiteTypes > 2 || blackTypes > 2)
		{
			// At least one side has more than 2 pieces, this is always enough
			// to theoretically mate.
			return true;
		}

		if (whiteTypes == 2 && blackTypes == 2)
		{
			if (!(has(typesPresent[0], PieceType::Bishop)
					&& has(typesPresent[1], PieceType::Bishop)))
			{
				// If both sides do not have King + Bishop but have two pieces,
				// mate is theoretically possible
				return true;
			}

			// Both sides have King + Bishop, we need to figure out if they are
			// of the same color
			std::optional<Color> bishopColor;
			for (uint8_t index = 0; index < 64; ++index)
			{
				const uint8_t code = squares[index];

				if (code == 0)
				{
					continue;
				}

				const Square sq = BoardAccess::squareOf(index);

				if (BoardAccess::typeOf(code) == PieceType::Bishop)
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
		const uint8_t relevantSideTypes
			= (whiteTypes > 1) ? typesPresent[0] : typesPresent[1];

		const uint8_t drawingTypes
			= static_cast<uint8_t>(
					(1 << static_cast<uint8_t>(PieceType::King))
					| (1 << static_cast<uint8_t>(PieceType::Knight))
					| (1 << static_cast<uint8_t>(PieceType::Bishop)));

		// A piece which is neither King, Bishop nor Knight means mate is
		// theoretically possible.
		return (relevantSideTypes & ~drawingTypes) != 0;
	}

}

std::optional<DrawReason> DrawEvaluator::reasonToDraw(
		const GameStage& stage,
		const std::map<std::string, uint8_t>& previouslyReachedPositions,
		bool drawOffered)
{
	const std::vector<PieceMove> allPossibleMoves
		= MoveValidator::allAvailableMoves(
				stage.board(),
				stage.enPassantTarget(),
				stage.castlingRights(),
				stage.activeColor());

	const bool inCheck
		= BoardAnalyzer::isInCheck(stage.board(), stage.activeColor());

	return reasonToDraw(
			stage,
			inCheck,
			allPossibleMoves,
			previouslyReachedPositions,
			drawOffered);
}

std::optional<DrawReason> DrawEvaluator::reasonToDraw(
		const GameStage& stage,
		const bool isInCheck,
		const std::vector<PieceMove>& allPossibleMoves,
		const std::map<std::string, uint8_t>& previouslyReachedPositions,
		bool drawOffered)
{
	if (stage.halfMovesSinceLastCaptureOrPawnAdvance() >= 150)
	{
		return { DrawReason::SeventyFiveMoveRule };
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
		return { DrawReason::FiveFoldRepetition };
	}

	if (allPossibleMoves.size() == 0 && !isInCheck)
	{
		return { DrawReason::StaleMate };
	}

	if (!internal::enoughMatingMaterial(stage.board()))
	{
		return { DrawReason::InsufficientMaterial };
	}

	if (internal::opponentHasOnlyKing(stage.board(), stage.activeColor()))
	{
		return { DrawReason::OpponentInsufficientMaterial };
	}

	if (drawOffered)
	{
		return { DrawReason::OfferedAndAccepted };
	}

	if (stage.halfMovesSinceLastCaptureOrPawnAdvance() >= 99)
	{
		return { DrawReason::FiftyMoveRule };
	}

	// A player can claim a draw if the current position has been reached at
	// least twice before, or if the position emerging from their move in
	// their current turn has appeared at least twice before. Hence, we need to
	// figure out if any of the positions which might emerge from this move
	// would cause 3-fold repetition
	if (timesPositionAppearedPreviously >= 2)
	{
		return { DrawReason::ThreeFoldRepetition };
	}

	// The look-ahead below can only ever fire on a position which has already
	// been reached twice. If no position has, the whole loop is dead and can
	// be skipped (which is the case on virtually every move of a normal
	// game)
	const bool anyPositionReachedTwice = std::any_of(
			previouslyReachedPositions.begin(),
			previouslyReachedPositions.end(),
			[](const auto& entry) { return entry.second >= 2; });

	if (!anyPositionReachedTwice)
	{
		return {};
	}

	for (const auto& move : allPossibleMoves)
	{
		// Only the repetition key of the hypothetical position is needed
		// here, so it is built directly rather than by constructing a whole
		// GameStage (which would generate a full FEN, derive the check status
		// and, through PlayedMove, the algebraic notation of the move).
		const Board nextBoard
			= BoardAnalyzer::makeMoveOnBoard(stage.board(), move);

		const std::string relevantFen = FenUtils::repetitionKey(
				nextBoard,
				oppositeColor(stage.activeColor()),
				updatedCastlingRights(stage.castlingRights(), move),
				MoveValidator::enPassantTarget(nextBoard, move));

		const auto it = previouslyReachedPositions.find(relevantFen);

		if (it != previouslyReachedPositions.end() && it->second >= 2)
		{
			return { DrawReason::ThreeFoldRepetition };
		}
	}

	return {};
}
