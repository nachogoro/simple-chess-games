#include <GameEngine.h>

#include "GameEngineImpl.h"

using namespace simplechess;

GameEngine::GameEngine()
	: mInternalGameEngine(new GameEngineImpl())
{ }

Game GameEngine::createGameFromFile(const std::string& inputFile)
{
	return mInternalGameEngine->createGameFromFile(inputFile);
}

Game GameEngine::createNewGame()
{
	return mInternalGameEngine->createNewGame();
}

Game GameEngine::deleteGame(uint32_t id)
{
	return mInternalGameEngine->deleteGame(id);

void GameEngine::exportGame(uint32_t id, const std::string& outFile)
{
	mInternalGameEngine->exportGame(id, outFile);
}

std::vector<PossibleMove> GameEngine::possibleMoves(
		uint32_t id, const Square& srcSquare)
{
	return mInternalGameEngine->possibleMoves(id, srcSquare);
}

Game GameEngine::makeMove(uint32_t id, const Move& move)
{
	return mInternalGameEngine->makeMove(id, move);
}
