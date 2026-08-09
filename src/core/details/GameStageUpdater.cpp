#include "GameStageUpdater.h"

#include "../Builders.h"
#include "BoardAnalyzer.h"
#include "MoveValidator.h"
#include "fen/FenUtils.h"

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

	const uint8_t updatedRights
		= details::updatedCastlingRights(stage.castlingRights(), move);

	const Board nextBoard = details::BoardAnalyzer::makeMoveOnBoard(stage.board(), move);
	const Color nextActiveColor = oppositeColor(stage.activeColor());
	const uint16_t nextHalfmoveClock = (move.piece().type() == PieceType::Pawn || playedMove.capturedPiece())
		? 0
		: stage.halfMovesSinceLastCaptureOrPawnAdvance() + 1;
	const uint16_t nextFullmoveCounter = stage.fullMoveCounter()
		+ ((stage.activeColor() == Color::Black) ? 1 : 0);

	// Calculate en passant target
	const std::optional<Square> enPassantTarget = MoveValidator::enPassantTarget(nextBoard, {move});

	return GameStageBuilder::build(
		nextBoard,
		nextActiveColor,
		updatedRights,
		nextHalfmoveClock,
		nextFullmoveCounter,
		enPassantTarget);
}
