#include "GameStage.h"

#include "details/BoardAnalyzer.h"

#include <cctype>
#include <sstream>

using namespace simplechess;

GameStage::GameStage(
		const Board& board,
		const Color toPlay,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const std::string& fen,
		const boost::optional<PlayedMove>& move)
	: mBoard(board),
	  mActiveColor(toPlay),
	  mCastlingRights(castlingRights),
	  mHalfmoveClock(halfmoveClock),
	  mFullmoveClock(fullmoveClock),
	  mFen(fen),
	  mMove(move)
{
	// TODO maybe not validate here
	//
	// Validate that the state is correct:
	// 1. One and only one King per side
	// 2. The color to move cannot be checking the opposite King
	// 3. If one type of castling is available, both the king and the rook
	// should be in place
	// 4. Move clocks are consistent
	uint8_t whiteKings = 0;
	uint8_t blackKings = 0;

	for (const auto& entry : board.occupiedSquares())
	{
		if (entry.second.type() == TYPE_KING)
		{
			if (entry.second.color() == COLOR_WHITE)
			{
				whiteKings++;
			}
			else
			{
				blackKings++;
			}
		}
	}

	if (whiteKings != 1 || blackKings != 1)
	{
		throw std::invalid_argument("Invalid number of kings on board");
	}

	if (details::BoardAnalyzer::isInCheck(board, oppositeColor(toPlay)))
	{
		throw std::invalid_argument("Color to move is already checking");
	}

	const Piece whiteKing = {TYPE_KING, COLOR_WHITE};
	const Piece blackKing = {TYPE_KING, COLOR_BLACK};
	const Piece whiteRook = {TYPE_ROOK, COLOR_WHITE};
	const Piece blackRook = {TYPE_ROOK, COLOR_BLACK};

	if ((castlingRights & CASTLING_RIGHT_WHITE_KINGSIDE)
			&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
				|| *board.pieceAt(Square::fromString("h1")) != whiteRook))
	{
		throw std::invalid_argument(
				"Kingside castling right for white is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_WHITE_QUEENSIDE)
			&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
				|| *board.pieceAt(Square::fromString("a1")) != whiteRook))
	{
		throw std::invalid_argument(
				"Queenside castling right for white is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_BLACK_KINGSIDE)
			&& (*board.pieceAt(Square::fromString("e8")) != blackKing
				|| *board.pieceAt(Square::fromString("h8")) != blackRook))
	{
		throw std::invalid_argument(
				"Kingside castling right for black is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_BLACK_QUEENSIDE)
			&& (*board.pieceAt(Square::fromString("e8")) != blackKing
				|| *board.pieceAt(Square::fromString("a8")) != blackRook))
	{
		throw std::invalid_argument(
				"Queenside castling right for black is inconsistent with board state");
	}
}

const Board& GameStage::board() const
{
	return mBoard;
}

const boost::optional<PlayedMove>& GameStage::move() const
{
	return mMove;
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
