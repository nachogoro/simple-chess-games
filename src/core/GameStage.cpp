#include <cpp/simplechess/GameStage.h>

using namespace simplechess;

GameStage::GameStage(
		const Board& board,
		const Color toPlay,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const std::string& fen,
		const std::optional<Square>& enPassantTarget,
		const CheckType checkStatus)
	: mBoard(board),
	  mActiveColor(toPlay),
	  mCastlingRights(castlingRights),
	  mHalfmoveClock(halfmoveClock),
	  mFullmoveClock(fullmoveClock),
	  mFen(fen),
	  mEnPassantTarget(enPassantTarget),
	  mCheckStatus(checkStatus)
{
	// Validation is now done at game creation time in GameManagerImpl::createGameFromFen
	// This constructor assumes the position is valid since it's only called from validated contexts
}

const Board& GameStage::board() const
{
	return mBoard;
}


Color GameStage::activeColor() const
{
	return mActiveColor;
}

uint8_t GameStage::castlingRights() const
{
	return mCastlingRights;
}

uint16_t GameStage::halfMovesSinceLastCaptureOrPawnAdvance() const
{
	return mHalfmoveClock;
}

uint16_t GameStage::fullMoveCounter() const
{
	return mFullmoveClock;
}

const std::string& GameStage::fen() const
{
	return mFen;
}

std::optional<Square> GameStage::enPassantTarget() const
{
	return mEnPassantTarget;
}

CheckType GameStage::checkStatus() const
{
	return mCheckStatus;
}
