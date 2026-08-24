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
		// Checkmate is impossible, by any series of legal moves, only when
		// what is left on the board is:
		//   - the two kings alone
		//   - a single bishop or knight besides them, on either side
		//   - bishops which all stand on squares of one colour
		//
		// The last of those holds however many bishops there are and
		// whoever owns them, so it covers one each as well as two on one
		// side. A pair covering both colours is not among them: it mates.
		uint8_t knights = 0;
		uint8_t bishops = 0;
		std::optional<Color> bishopSquareColor;
		bool bishopsOnBothColors = false;

		const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

		for (uint8_t index = 0; index < 64; ++index)
		{
			const uint8_t code = squares[index];

			if (code == 0)
			{
				continue;
			}

			switch (BoardAccess::typeOf(code))
			{
				case PieceType::King:
					break;

				case PieceType::Knight:
					++knights;
					break;

				case PieceType::Bishop:
				{
					++bishops;

					const Color squareColor
						= BoardAccess::squareOf(index).color();

					if (!bishopSquareColor)
					{
						bishopSquareColor = {squareColor};
					}
					else if (*bishopSquareColor != squareColor)
					{
						bishopsOnBothColors = true;
					}

					break;
				}

				default:
					// A pawn, a rook or a queen is always enough for
					// somebody to be mated
					return true;
			}
		}

		if (knights + bishops <= 1)
		{
			return false;
		}

		return knights != 0 || bishopsOnBothColors;
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
