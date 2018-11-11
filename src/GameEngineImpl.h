#ifndef GAMEENGINEIMPL_H_65D47FAD_12CB_4775_8BBD_42343060A1E5
#define GAMEENGINEIMPL_H_65D47FAD_12CB_4775_8BBD_42343060A1E5

#include <Game.h>
#include <Move.h>
#include <PossibleMove.h>
#include <Square.h>

#include "GameImpl.h"

#include <atomic>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace simplechess
{
	class GameEngineImpl
	{
		public:
			GameEngineImpl();

			Game createGameFromFile(const std::string& inputFile);

			Game createNewGame();

			Game deleteGame(uint32_t id);

			void exportGame(uint32_t id, const std::string& outFile) const;

			std::vector<PossibleMove> possibleMoves(
					uint32_t id, const Square& srcSquare) const;

			Game makeMove(uint32_t id, const Move& move);

		private:
			std::map<uint32_t, std::shared_ptr<GameImpl>> mAllGames;

			// Mutex for when inserting a new game into mAllGames
			mutable std::mutex mAdditionMutex;
			static std::atomic<uint32_t> mNextGameId;
	};
};

#endif
