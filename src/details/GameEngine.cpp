#include <GameEngine.h>

#include "GameEngineImpl.h"

using namespace simplechess;

GameEngine::GameEngine()
	: mImpl(new GameEngineImpl())
{ }

Game GameEngine::createGameFromFile(const std::string& inputFile)
{
	return mImpl->createGameFromFile(inputFile);
}

Game GameEngine::createNewGame()
{
	return mImpl->createNewGame();
}

void GameEngine::deleteGame(uint32_t id)
{
	mImpl->deleteGame(id);
}

void GameEngine::exportGame(uint32_t id, const std::string& outFile) const
{
	mImpl->exportGame(id, outFile);
}

std::vector<PossibleMove> GameEngine::possibleMoves(
		uint32_t id, const Square& srcSquare) const
{
	return mImpl->possibleMoves(id, srcSquare);
}

Game GameEngine::makeMove(uint32_t id, const Move& move)
{
	return mImpl->makeMove(id, move);
}
