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
	const std::shared_ptr<LockedGameImpl> game
		= LockedGameImpl::parseFromFile(inputFile, gameId);

	std::lock_guard<std::mutex> lock(mAdditionMutex);
	mAllGames.insert(
			{gameId,
			std::shared_ptr<LockedGameImpl>(new LockedGameImpl(gameId))});
	return game->toPublicGame();
}

Game GameEngineImpl::createNewGame()
{
	const uint32_t gameId = mNextGameId++;
	const std::shared_ptr<LockedGameImpl> game(new LockedGameImpl(gameId));
	std::lock_guard<std::mutex> lock(mAdditionMutex);
	mAllGames.insert({gameId, game});
	return game->toPublicGame();
}

void GameEngineImpl::deleteGame(uint32_t id)
{
	std::lock_guard<std::mutex> mapLock(mAdditionMutex);
	if (mAllGames.count(id) == 0)
	{
		throw Exception(
				"No game with id " + std::to_string(id) + " to be deleted");
	}

	mAllGames.erase(id);
}

void GameEngineImpl::exportGame(uint32_t id, const std::string& outFile) const
{
	std::shared_ptr<LockedGameImpl> game;
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

	game->exportToFile(outFile);
}

std::vector<PossibleMove> GameEngineImpl::possibleMoves(
		uint32_t id, const Square& srcSquare) const
{
	std::shared_ptr<LockedGameImpl> game;
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

	return game->possibleMoves(srcSquare);
}

Game GameEngineImpl::makeMove(uint32_t id, const Move& move)
{
	std::shared_ptr<LockedGameImpl> game;
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

	return game->makeMove(move);
}
