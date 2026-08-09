#include "GameStageUpdater.h"

#include "../Builders.h"
#include "BoardAnalyzer.h"
#include "MoveValidator.h"
#include "fen/FenUtils.h"

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	/**
	 * The components of the stage which follows \p stage once \p move has
	 * been played, i.e. everything except the derived check status.
	 */
	struct NextStageComponents
	{
		Board board;
		Color activeColor;
		uint8_t castlingRights;
		uint16_t halfmoveClock;
		uint16_t fullmoveCounter;
		std::optional<Square> enPassantTarget;
	};

	NextStageComponents nextStageComponents(
			const GameStage& stage,
			const PieceMove& move)
	{
		const uint8_t updatedRights
			= details::updatedCastlingRights(stage.castlingRights(), move);

		const Board nextBoard
			= details::BoardAnalyzer::makeMoveOnBoard(stage.board(), move);

		// The halfmove clock resets on a pawn move or a capture. Looking the
		// destination square up on the board answers that directly; building
		// a full PlayedMove (which generates algebraic notation, and with it
		// a complete legal move generation) just to read this back would be
		// enormously more expensive.
		const bool resetsHalfmoveClock
			= move.piece().type() == PieceType::Pawn
				|| stage.board().pieceAt(move.dst()).has_value();

		return {
			nextBoard,
			oppositeColor(stage.activeColor()),
			updatedRights,
			static_cast<uint16_t>(resetsHalfmoveClock
				? 0
				: stage.halfMovesSinceLastCaptureOrPawnAdvance() + 1),
			static_cast<uint16_t>(stage.fullMoveCounter()
				+ ((stage.activeColor() == Color::Black) ? 1 : 0)),
			details::MoveValidator::enPassantTarget(nextBoard, move)};
	}
}

GameStage GameStageUpdater::makeMove(
		const GameStage& stage,
		const PieceMove& move)
{
	const internal::NextStageComponents next
		= internal::nextStageComponents(stage, move);

	return GameStageBuilder::build(
		next.board,
		next.activeColor,
		next.castlingRights,
		next.halfmoveClock,
		next.fullmoveCounter,
		next.enPassantTarget);
}

UpdatedStage GameStageUpdater::makeMoveWithAnalysis(
		const GameStage& stage,
		const PieceMove& move)
{
	const internal::NextStageComponents next
		= internal::nextStageComponents(stage, move);

	PositionAnalysis analysis = details::analyzePosition(
			next.board,
			next.activeColor,
			next.enPassantTarget,
			next.castlingRights);

	GameStage nextStage = GameStageBuilder::build(
		next.board,
		next.activeColor,
		next.castlingRights,
		next.halfmoveClock,
		next.fullmoveCounter,
		next.enPassantTarget,
		analysis.checkType);

	return {std::move(nextStage), std::move(analysis)};
}
