#include "GameStageUpdater.h"

#include "../Builders.h"
#include "BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

GameStage GameStageUpdater::makeMove(
		const GameStage& stage,
		const PieceMove& move,
		const bool offerDraw)
{
	const PlayedMove playedMove = PlayedMoveBuilder::build(
			stage.board(),
			move,
			offerDraw);

	uint8_t updatedCastlingRights = stage.castlingRights();

	if (move.piece().type() == PieceType::King)
	{
		// Once the king moves, castling is no longer allowed
		if (move.piece().color() == Color::White)
		{
			updatedCastlingRights &= ~CastlingRight::WhiteKingSide;
			updatedCastlingRights &= ~CastlingRight::WhiteQueenSide;
		}
		else
		{
			updatedCastlingRights &= ~CastlingRight::BlackKingSide;
			updatedCastlingRights &= ~CastlingRight::BlackQueenSide;
		}
	}

	// If the move starts or ends in a rook's original square, castling rights
	// are lost
	if (move.src() == Square::fromString("a1")
			|| move.dst() == Square::fromString("a1"))
	{
		updatedCastlingRights &= ~CastlingRight::WhiteQueenSide;
	}

	if (move.src() == Square::fromString("h1")
			|| move.dst() == Square::fromString("h1"))
	{
		updatedCastlingRights &= ~CastlingRight::WhiteKingSide;
	}

	if (move.src() == Square::fromString("a8")
			|| move.dst() == Square::fromString("a8"))
	{
		updatedCastlingRights &= ~CastlingRight::BlackQueenSide;
	}

	if (move.src() == Square::fromString("h8")
			|| move.dst() == Square::fromString("h8"))
	{
		updatedCastlingRights &= ~CastlingRight::BlackKingSide;
	}

	return GameStageBuilder::build(
			details::BoardAnalyzer::makeMoveOnBoard(stage.board(), move),
			oppositeColor(stage.activeColor()),
			updatedCastlingRights,
			(move.piece().type() == PieceType::Pawn || playedMove.capturedPiece())
				? 0
				: stage.halfMovesSinceLastCaptureOrPawnAdvance() + 1,
			stage.fullMoveCounter()
				+ ((stage.activeColor() == Color::Black)
					? 1
					: 0),
			playedMove);
}
