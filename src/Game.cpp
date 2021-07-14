#include <Game.h>

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

namespace internal
{
	std::map<std::string, uint8_t> getPositionsMap(
			const std::vector<GameStage>& history)
	{
		std::map<std::string, uint8_t> result;

		for (const auto& stage : history)
		{
			const std::string repetitionFen
				= details::FenUtils::fenForRepetitions(stage.fen());

			if (result.count(repetitionFen))
			{
				result.at(repetitionFen)++;
			}
			else
			{
				result.insert({repetitionFen, 1});
			}
		}

		return result;
	}
}

Game::Game(
		const GameState gameState,
		const boost::optional<DrawReason>& drawReason,
		const std::vector<GameStage>& history,
		const std::set<PieceMove>& allAvailableMoves,
		const boost::optional<DrawReason>& reasonToClaimDraw)
	: mGameState(gameState),
	  mReasonGameWasDrawn(drawReason),
	  mHistory(history),
	  mAllAvailableMoves(allAvailableMoves),
	  mReasonToClaimDraw(reasonToClaimDraw)
{
	if ((gameState == GAME_STATE_DRAWN && !drawReason)
			|| (gameState != GAME_STATE_DRAWN && drawReason))
	{
		throw std::invalid_argument(
				"Inconsistent arguments related to draw reason");
	}
}

const GameStage& Game::currentStage() const
{
	return mHistory[mHistory.size() - 1];
}

GameState Game::gameState() const
{
	return mGameState;
}

DrawReason Game::drawReason() const
{
	if (gameState() != GAME_STATE_DRAWN)
	{
		throw IllegalStateException(
				"Cannot request draw reason of non-drawn game");
	}

	return *mReasonGameWasDrawn;
}

const std::vector<GameStage>& Game::gameHistory() const
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

const boost::optional<DrawReason>& Game::reasonToClaimDraw() const
{
	if (gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	return mReasonToClaimDraw;
}
