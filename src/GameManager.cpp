#include <GameManager.h>

#include "GameManagerImpl.h"

using namespace simplechess;

GameManager::GameManager()
	: mImpl(std::make_unique<GameManagerImpl>())
{
}


Game GameManager::createNewGame() const
{
	return mImpl->createNewGame();
}

Game GameManager::createGameFromFen(const std::string& fen) const
{
	return mImpl->createGameFromFen(fen);
}

Game GameManager::makeMove(
		const Game& game,
		const PieceMove& move,
		bool offerDraw) const
{
	return mImpl->makeMove(game, move, offerDraw);
}

Game GameManager::claimDraw(const Game& game) const
{
	return mImpl->claimDraw(game);
}

Game GameManager::resign(const Game& game, Color resigningPlayer) const
{
	return mImpl->resign(game, resigningPlayer);
}
