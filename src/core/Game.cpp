#include <simplechess/Game.h>

#include "details/BoardAnalyzer.h"
#include "details/DrawEvaluator.h"
#include "details/MoveValidator.h"
#include "details/fen/FenParser.h"
#include "details/fen/FenUtils.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tuple/tuple.hpp>

#include <algorithm>

using namespace simplechess;

Game::Game(
		const GameState gameState,
		const std::optional<DrawReason>& drawReason,
		std::vector<std::pair<GameStage, PlayedMove>> history,
		GameStage currentStage,
		std::vector<PieceMove> allAvailableMoves,
		const std::optional<DrawReason>& reasonToClaimDraw,
		const DrawEnforcement drawEnforcement,
		std::map<std::string, uint8_t> previouslyReachedPositions)
	: mGameState(gameState),
	  mReasonGameWasDrawn(drawReason),
	  mHistory(std::move(history)),
	  mCurrentStage(std::move(currentStage)),
	  mAllAvailableMoves(std::move(allAvailableMoves)),
	  mReasonToClaimDraw(reasonToClaimDraw),
	  mDrawEnforcement(drawEnforcement),
	  mPreviouslyReachedPositions(std::move(previouslyReachedPositions))
{
	if ((gameState == GameState::Drawn && !drawReason)
			|| (gameState != GameState::Drawn && drawReason))
	{
		throw std::invalid_argument(
				"Inconsistent arguments related to draw reason");
	}

	if (gameState != GameState::Playing && reasonToClaimDraw)
	{
		throw std::invalid_argument(
				"A finished game cannot have a reason to claim a draw");
	}
}

const GameStage& Game::currentStage() const
{
	return mCurrentStage;
}

GameState Game::gameState() const
{
	return mGameState;
}

DrawReason Game::drawReason() const
{
	if (gameState() != GameState::Drawn)
	{
		throw IllegalStateException(
				"Cannot request draw reason of non-drawn game");
	}

	return *mReasonGameWasDrawn;
}

const std::vector<std::pair<GameStage, PlayedMove>>& Game::history() const
{
	return mHistory;
}

Color Game::activeColor() const
{
	return currentStage().activeColor();
}

std::vector<PieceMove> Game::availableMovesForPiece(const Square& square) const
{
	std::vector<PieceMove> result;

	for (const auto& move : allAvailableMoves())
	{
		if (move.src() == square)
		{
			result.push_back(move);
		}
	}

	return result;
}

const std::vector<PieceMove>& Game::allAvailableMoves() const
{
	return mAllAvailableMoves;
}

const std::optional<DrawReason>& Game::reasonToClaimDraw() const
{
	if (gameState() != GameState::Playing)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	return mReasonToClaimDraw;
}

DrawEnforcement Game::drawEnforcement() const
{
	return mDrawEnforcement;
}

const std::map<std::string, uint8_t>& Game::previouslyReachedPositions() const
{
	return mPreviouslyReachedPositions;
}
