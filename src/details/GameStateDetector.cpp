#include "GameStateDetector.h"

#include "details/BoardAnalyzer.h"
#include "details/DrawEvaluator.h"
#include "details/MoveValidator.h"

#include <boost/tuple/tuple.hpp>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	boost::optional<Square> enPassantTarget(
			const GameStage& stage)
	{
		const boost::optional<PlayedMove>& move = stage.move();

		if (move
				&& move->pieceMove().piece().type() == TYPE_PAWN
				&& abs(move->pieceMove().dst().rank()
					- move->pieceMove().src().rank()) == 2)
		{
			return { Square::fromRankAndFile(
					((move->pieceMove().piece().color() == COLOR_WHITE)
					 ? 3
					 : 6),
					move->pieceMove().dst().file()) };
		}

		return {};
	}

	boost::tuple<GameState, boost::optional<DrawReason>> inferGameStateFromStage(
			const GameStage& stage,
			const bool inCheck,
			const std::set<PieceMove> allPossibleMoves,
			const boost::optional<DrawReason> reasonToClaimDraw)
	{
		if (inCheck)
		{
			if (allPossibleMoves.size() == 0)
			{
				// If the active color can't move and is in check, it is check
				// mate
				return {
					stage.activeColor() == COLOR_WHITE
						? GAME_STATE_BLACK_WON
						: GAME_STATE_WHITE_WON,
						{} };
			}
		}

		if (!reasonToClaimDraw
				|| (*reasonToClaimDraw != DRAW_REASON_STALEMATE
						&& *reasonToClaimDraw != DRAW_REASON_75_MOVE_RULE
						&& *reasonToClaimDraw != DRAW_REASON_FIVE_FOLD_REPETITION
						&& *reasonToClaimDraw != DRAW_REASON_INSUFFICIENT_MATERIAL))
		{
			// If there is a reason to draw, it is not automatically applied,
			// so the game is still playable
			return {GAME_STATE_PLAYING, {}};
		}

		// There is a mandatory draw reason, so the game is drawn
		return { GAME_STATE_DRAWN, reasonToClaimDraw };
	}
}

GameStateInformation GameStateDetector::detect(
		const GameStage& stage,
		const std::map<std::string, uint8_t>& previouslyReachedPositions)
{
	const bool inCheck = BoardAnalyzer::isInCheck(
			stage.board(),
			stage.activeColor());

	const std::set<PieceMove> availableMoves
		= MoveValidator::allAvailableMoves(
				stage.board(),
				internal::enPassantTarget(stage),
				stage.castlingRights(),
				stage.activeColor());

	const CheckType checkType = (inCheck
		? ((availableMoves.size() == 0)
				? CHECKMATE
				: CHECK)
		: NO_CHECK);

	const boost::optional<DrawReason> reasonToClaimDraw
		= details::DrawEvaluator::reasonToDraw(
				stage,
				inCheck,
				availableMoves,
				previouslyReachedPositions);

	const boost::tuple<GameState, boost::optional<DrawReason>> gameState
		= internal::inferGameStateFromStage(
			stage,
			inCheck,
			availableMoves,
			reasonToClaimDraw);

	return {
		gameState.get<0>(),
			checkType,
			availableMoves,
			gameState.get<1>(),
			reasonToClaimDraw };

}
