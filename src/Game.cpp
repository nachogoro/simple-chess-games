#include <Game.h>
// TODO remove
#include <Exception.h>


using namespace simplechess;

Game::Game(
		const uint32_t id,
		const Board& board,
		const std::vector<Move>& moveHistory,
		const std::vector<Piece>& capturedPieces)
	: mId(id),
	  mBoard(board),
	  mMoveHistory(moveHistory),
	  mCapturedPieces(capturedPieces)
{  }


uint32_t Game::id() const
{
	return mId;
}


const Board& Game::board() const
{
	return mBoard;
}


const std::vector<Move>& Game::moveHistory() const
{
	return mMoveHistory;
}


const std::vector<Piece>& Game::capturedPieces() const
{
	return mCapturedPieces;
}


boost::optional<GameEnding> Game::gameWinner() const
{
	const Move& lastMove = mMoveHistory.back();
	// TODO check if it's a final move with the algebraic motor
	throw Exception("");
}


boost::optional<Color> Game::nextPlayer() const
{
	return mMoveHistory.size() % 2 == 0 ? COLOR_WHITE : COLOR_BLACK;
}
