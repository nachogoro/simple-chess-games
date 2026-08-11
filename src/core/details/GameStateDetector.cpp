#include "GameStateDetector.h"

#include "BoardAnalyzer.h"
#include "DrawEvaluator.h"
#include "MoveValidator.h"

#include <boost/tuple/tuple.hpp>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	std::optional<Square> enPassantTarget(
			const GameStage& stage)
	{
		return stage.enPassantTarget();
	}

	boost::tuple<GameState, std::optional<DrawReason>> inferGameStateFromStage(
			const GameStage& stage,
			const bool inCheck,
			const std::vector<PieceMove>& allPossibleMoves,
			const std::optional<DrawReason>& reasonToClaimDraw,
			const DrawEnforcement drawEnforcement)
	{
		if (inCheck)
		{
			if (allPossibleMoves.size() == 0)
			{
				// If the active color can't move and is in check, it is check
				// mate
				return {
					stage.activeColor() == Color::White
						? GameState::BlackWon
						: GameState::WhiteWon,
						{} };
			}
		}

		if (reasonToClaimDraw && *reasonToClaimDraw == DrawReason::StaleMate)
		{
			// Stalemate is always enforced regardless of draw enforcement mode
			return { GameState::Drawn, reasonToClaimDraw };
		}

		if (drawEnforcement == DrawEnforcement::Automatic
				&& reasonToClaimDraw
				&& (*reasonToClaimDraw == DrawReason::SeventyFiveMoveRule
						|| *reasonToClaimDraw == DrawReason::FiveFoldRepetition
						|| *reasonToClaimDraw == DrawReason::InsufficientMaterial))
		{
			// Mandatory draw reason with automatic enforcement
			return { GameState::Drawn, reasonToClaimDraw };
		}

		return {GameState::Playing, {}};
	}
}

GameStateInformation GameStateDetector::detect(
		const GameStage& stage,
		bool drawOffered,
		const std::map<std::string, uint8_t>& previouslyReachedPositions,
		const DrawEnforcement drawEnforcement)
{
	return detect(
			stage,
			details::analyzePosition(
					stage.board(),
					stage.activeColor(),
					internal::enPassantTarget(stage),
					stage.castlingRights()),
			drawOffered,
			previouslyReachedPositions,
			drawEnforcement);
}

GameStateInformation GameStateDetector::detect(
		const GameStage& stage,
		const PositionAnalysis& analysis,
		bool drawOffered,
		const std::map<std::string, uint8_t>& previouslyReachedPositions,
		const DrawEnforcement drawEnforcement)
{
	const std::optional<DrawReason> reasonToClaimDraw
		= details::DrawEvaluator::reasonToDraw(
				stage,
				analysis.inCheck(),
				analysis.legalMoves,
				previouslyReachedPositions,
				drawOffered);

	const boost::tuple<GameState, std::optional<DrawReason>> gameState
		= internal::inferGameStateFromStage(
			stage,
			analysis.inCheck(),
			analysis.legalMoves,
			reasonToClaimDraw,
			drawEnforcement);

	// A draw can only be claimed while there is still a game to claim it in.
	// The reason is worked out from the position alone, which says nothing
	// about whether the game has just ended, so a checkmate delivered on the
	// hundredth halfmove would otherwise report the fifty-move rule as
	// claimable in a game somebody has already won.
	const bool stillBeingPlayed = gameState.get<0>() == GameState::Playing;

	return {
		gameState.get<0>(),
			analysis.checkType,
			analysis.legalMoves,
			gameState.get<1>(),
			stillBeingPlayed
				? reasonToClaimDraw
				: std::optional<DrawReason>() };

}
