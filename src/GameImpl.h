#ifndef GAMEIMPL_H_71970E14_585C_4D5E_97CB_936CA79B9C9F
#define GAMEIMPL_H_71970E14_585C_4D5E_97CB_936CA79B9C9F

#include "MoveHistory.h"

#include <Board.h>
#include <Game.h>
#include <PossibleMove.h>

#include <memory>
#include <mutex>

namespace simplechess
{
	class GameImpl
	{
		public:
			explicit GameImpl(uint32_t id);

			GameImpl(
					uint32_t id,
					const Board& board,
					const std::vector<Move>& moveHistory,
					const std::vector<Piece>& capturedPieces);

			static GameImpl parseFromFile(
					const std::string& file, uint32_t id);

			Game toPublicGame() const;

			void exportToFile(const std::string& outFile) const;

			std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare) const;

			Game makeMove(const Move& move);

		private:
			uint32_t mId;
			Board mBoard;
			MoveHistory mMoveHistory;
			std::vector<Piece> mCapturedPieces;
	};
}

#endif
