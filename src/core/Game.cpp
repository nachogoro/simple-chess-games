#include <cpp/simplechess/Game.h>

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

// TODO Compute things only once (e.g. verifying the kind of check a move would
// imply, or the available moves from a position).
//
// TODO In order to be able to move to the public header include/ vs
// implementation src/ directories, make public facing objects simply contain
// pre-computed information passed to them through their constructor, and leave
// all "computing" code (e.g. makeMove()) to a singleton object making use of
// the pImpl idiom

Game::Game(
		const GameState gameState,
		const std::optional<DrawReason>& drawReason,
		const std::vector<std::pair<GameStage, PlayedMove>>& history,
		const GameStage& currentStage,
		const std::set<PieceMove>& allAvailableMoves,
		const std::optional<DrawReason>& reasonToClaimDraw)
	: mGameState(gameState),
	  mReasonGameWasDrawn(drawReason),
	  mHistory(history),
	  mCurrentStage(currentStage),
	  mAllAvailableMoves(allAvailableMoves),
	  mReasonToClaimDraw(reasonToClaimDraw)
{
	if ((gameState == GameState::Drawn && !drawReason)
			|| (gameState != GameState::Drawn && drawReason))
	{
		throw std::invalid_argument(
				"Inconsistent arguments related to draw reason");
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

std::set<PieceMove> Game::availableMovesForPiece(const Square& square) const
{
	std::set<PieceMove> result;

	for (const auto& move : allAvailableMoves())
	{
		if (move.src() == square)
		{
			result.insert(move);
		}
	}

	return result;
}

const std::set<PieceMove>& Game::allAvailableMoves() const
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
