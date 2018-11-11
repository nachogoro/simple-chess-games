#include "GameImpl.h"

using namespace simplechess;

GameImpl::GameImpl(uint32_t id)
	: mId(id),
	  mBoard(BoardImpl::createDefaultBoard()),
	  mMoveHistory()
{
}

GameImpl::GameImpl(
		uint32_t id,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory,
		const std::vector<Move>& capturedPieces)
	: mId(id),
	  mBoard(board),
	  mMoveHistory(moveHistory),
	  mCapturedPieces(capturedPieces)
{
}

std::unique_ptr<GameImpl> GameImpl::parseFromFile(
		const std::string& /*file*/, uint32_t id)
{
	// TODO
	return std::unique_ptr<GameImpl>(new GameImpl(id));
}

Game GameImpl::toPublicGame() const
{
	return Game(mId, mBoard.toPublicBoard(), mMoveHistory, mCapturedPieces);
}

std::mutex& GameImpl::mutex() const
{
	// TODO separate it from GameImpl
	return mGameMutex;
}

void GameImpl::exportToFile(const std::string& outFile) const
{
	// TODO
}

std::vector<PossibleMove> GameImpl::possibleMoves(
		const Square& srcSquare) const
{
	return mBoard.pieceAt(srcSquare).possibleMoves();
}

Game GameImpl::makeMove(const Move& move)
{
	// TODO
	return {mId};
}
