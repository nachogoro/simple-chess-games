#include "GameEngineImpl.h"

#include <Exception.h>

using namespace simplechess;

std::atomic<uint32_t> GameEngineImpl::mNextGameId{0};

GameEngineImpl::GameEngineImpl()
{
}

Game GameEngineImpl::createGameFromFile(const std::string& inputFile)
{
	const uint32_t gameId = mNextGameId++;
	const std::shared_ptr<GameImpl> game
		= GameImpl::parseFromFile(inputFile, gameId);

	std::lock_guard<std::mutex> lock(mAdditionMutex);
	mAllGames[gameId] = game;
	return game->toPublicGame();
}

Game GameEngineImpl::createNewGame()
{
	const uint32_t gameId = mNextGameId++;
	const std::shared_ptr<GameImpl> game(new GameImpl(gameId));
	std::lock_guard<std::mutex> lock(mAdditionMutex);
	mAllGames[gameId] = game;
	return game->toPublicGame();
}

Game GameEngineImpl::deleteGame(uint32_t id)
{
	std::lock_guard<std::mutex> mapLock(mAdditionMutex);
	if (mAllGames.count(id) == 0)
	{
		throw Exception(
				"No game with id " + std::to_string(id) + " to be deleted");
	}

	const Game game = mAllGames.at(id)->toPublicGame();
	mAllGames.erase(id);
	return game;
}

void GameEngineImpl::exportGame(uint32_t id, const std::string& outFile) const
{
	std::shared_ptr<GameImpl> game;
	{
		std::lock_guard<std::mutex> mapLock(mAdditionMutex);
		if (mAllGames.count(id) == 0)
		{
			throw Exception(
					"No game with id " + std::to_string(id)
					+ " to be exported");
		}
		game = mAllGames.at(id);
	}

	std::lock_guard<std::mutex> gameLock(game->mutex());

	game->exportToFile(outFile);
}

std::vector<PossibleMove> GameEngineImpl::possibleMoves(
		uint32_t id, const Square& srcSquare) const
{
	std::shared_ptr<GameImpl> game;
	{
		std::lock_guard<std::mutex> mapLock(mAdditionMutex);
		if (mAllGames.count(id) == 0)
		{
			throw Exception(
					"No game with id " + std::to_string(id)
					+ " to check possible moves");
		}
		game = mAllGames.at(id);
	}

	std::lock_guard<std::mutex> gameLock(game->mutex());

	return game->possibleMoves(srcSquare);
}

Game GameEngineImpl::makeMove(uint32_t id, const Move& move)
{
	std::shared_ptr<GameImpl> game;
	{
		std::lock_guard<std::mutex> mapLock(mAdditionMutex);
		if (mAllGames.count(id) == 0)
		{
			throw Exception(
					"No game with id " + std::to_string(id)
					+ " to make a move");
		}
		game = mAllGames.at(id);
	}

	std::lock_guard<std::mutex> gameLock(game->mutex());

	return game->makeMove(move);
}
