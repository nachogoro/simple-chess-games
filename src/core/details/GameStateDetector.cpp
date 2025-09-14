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
			const std::set<PieceMove> allPossibleMoves,
			const std::optional<DrawReason> reasonToClaimDraw)
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

		if (!reasonToClaimDraw
				|| (*reasonToClaimDraw != DrawReason::StaleMate
						&& *reasonToClaimDraw != DrawReason::SeventyFiveMoveRule
						&& *reasonToClaimDraw != DrawReason::FiveFoldRepetition
						&& *reasonToClaimDraw != DrawReason::InsufficientMaterial))
		{
			// If there is a reason to draw, it is not automatically applied,
			// so the game is still playable
			return {GameState::Playing, {}};
		}

		// There is a mandatory draw reason, so the game is drawn
		return { GameState::Drawn, reasonToClaimDraw };
	}
}

GameStateInformation GameStateDetector::detect(
		const GameStage& stage,
		bool drawOffered,
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
				? CheckType::CheckMate
				: CheckType::Check)
		: CheckType::NoCheck);

	const std::optional<DrawReason> reasonToClaimDraw
		= details::DrawEvaluator::reasonToDraw(
				stage,
				inCheck,
				availableMoves,
				previouslyReachedPositions,
				drawOffered);

	const boost::tuple<GameState, std::optional<DrawReason>> gameState
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
