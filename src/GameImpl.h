#ifndef GAMEIMPL_H_71970E14_585C_4D5E_97CB_936CA79B9C9F
#define GAMEIMPL_H_71970E14_585C_4D5E_97CB_936CA79B9C9F

#include "BoardImpl.h"

#include <Game.h>
#include <PossibleMove.h>

#include <memory>
#include <mutex>

namespace simplechess
{
	class GameImpl
	{
		public:
			GameImpl(uint32_t id);

			static std::unique_ptr<GameImpl> parseFromFile(
					const std::string& file, uint32_t id);

			Game toPublicGame() const;

			void exportToFile(const std::string& outFile) const;

			std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare) const;

			Game makeMove(const Move& move);

			std::mutex& mutex() const;
		private:
			GameImpl(
					uint32_t id,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory,
					const std::vector<Move>& capturedPieces);

		private:
			uint32_t mId;
			BoardImpl mBoard;
			std::vector<Move> mMoveHistory;
			std::vector<Piece> mCapturedPieces;
			mutable std::mutex mGameMutex;
	};
}

#endif
