#ifndef LOCKED_GAMEIMPL_H_0279ACFC_FC37_4C84_BCE4_7795818DC34C
#define LOCKED_GAMEIMPL_H_0279ACFC_FC37_4C84_BCE4_7795818DC34C

#include "GameImpl.h"
#include <mutex>

namespace simplechess
{
	/**
	 * \brief Wrapper around GameImpl for thread-safe operations.
	 */
	class LockedGameImpl
	{
		public:
			explicit LockedGameImpl(uint32_t id);

			explicit LockedGameImpl(const GameImpl&& game);

			static std::unique_ptr<LockedGameImpl> parseFromFile(
					const std::string& file, uint32_t id);

			Game toPublicGame() const;

			void exportToFile(const std::string& outFile) const;

			std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare) const;

			Game makeMove(const Move& move);
		private:
			LockedGameImpl(
					uint32_t id,
					const Board& board,
					const std::vector<Move>& moveHistory,
					const std::vector<Piece>& capturedPieces);

		private:
			mutable std::mutex mMutex;
			GameImpl mGame;
	};
}


#endif
