#include "GameStageUpdater"

#include "BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;


GameStage GameStageUpdater::makeMove(
		const GameStage& stage,
		const PieceMove& move,
		const bool offerDraw)
{
	const PlayedMove playedMove = PlayedMove::instantiate(
			stage.board(),
			move,
			offerDraw);

	uint8_t updatedCastlingRights = castlingRights();

	if (move.piece().type() == TYPE_KING)
	{
		// Once the king moves, castling is no longer allowed
		if (move.piece().color() == COLOR_WHITE)
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_KINGSIDE;
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_QUEENSIDE;
		}
		else
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_KINGSIDE;
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_QUEENSIDE;
		}
	}

	if (move.piece().type() == TYPE_ROOK)
	{
		// If a rook moves or is captured, clear the castling rights of that
		// side. It might already be cleared, but that's not an issue
		if (move.src() == Square::fromString("a1")
				|| move.dst() == Square::fromString("a1"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_QUEENSIDE;
		}

		if (move.src() == Square::fromString("h1")
				|| move.dst() == Square::fromString("h1"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_KINGSIDE;
		}

		if (move.src() == Square::fromString("a8")
				|| move.dst() == Square::fromString("a8"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_QUEENSIDE;
		}

		if (move.src() == Square::fromString("h8")
				|| move.dst() == Square::fromString("h8"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_KINGSIDE;
		}
	}

	return GameStage(
			details::BoardAnalyzer::makeMoveOnBoard(board(), move),
			oppositeColor(activeColor()),
			updatedCastlingRights,
			(move.piece().type() == TYPE_PAWN || playedMove.capturedPiece())
				? 0
				: halfMovesSinceLastCaptureOrPawnAdvance() + 1,
			fullMoveCounter() + (activeColor() == COLOR_BLACK ? 1 : 0),
			playedMove);
}
