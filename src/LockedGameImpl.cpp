#include "LockedGameImpl.h"

using namespace simplechess;

LockedGameImpl::LockedGameImpl(uint32_t id)
	: mGame(id)
{  }


LockedGameImpl::LockedGameImpl(const GameImpl&& game)
	: mGame(game)
{  }


LockedGameImpl::LockedGameImpl(
		uint32_t id,
		const Board& board,
		const std::vector<Move>& moveHistory,
		const std::vector<Piece>& capturedPieces)
	: mGame(id, board, moveHistory, capturedPieces)
{  }


std::unique_ptr<LockedGameImpl> LockedGameImpl::parseFromFile(
		const std::string& file, uint32_t id)
{
	return std::unique_ptr<LockedGameImpl>(
			new LockedGameImpl(GameImpl::parseFromFile(file, id)));
}


Game LockedGameImpl::toPublicGame() const
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mGame.toPublicGame();
}


void LockedGameImpl::exportToFile(const std::string& outFile) const
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mGame.exportToFile(outFile);
}


std::vector<PossibleMove> LockedGameImpl::possibleMoves(
		const Square& srcSquare) const
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mGame.possibleMoves(srcSquare);
}


Game LockedGameImpl::makeMove(const Move& move)
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mGame.makeMove(move);
}
