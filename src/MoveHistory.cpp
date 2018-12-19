#include "MoveHistory.h"
#include "BoardUtils.h"

#include <Exception.h>

using namespace simplechess;

Board MoveHistory::sDefaultBoard = BoardUtils::createDefaultBoard();

MoveHistory::MoveHistory()
{  }


MoveHistory::MoveHistory(const std::vector<Move>& src)
	: mMoves(src)
{  }


void MoveHistory::addNewMove(const Move& move)
{
	mMoves.push_back(move);
}


bool MoveHistory::hasEverMoved(const Square& square) const
{
	const boost::optional<Piece> piece = sDefaultBoard.pieceAt(square);
	if (!piece)
	{
		throw Exception(
				"No piece is originally at square " + square.toString());
	}

	// Rooks can be moved without showing up in a source square in the case
	// of castling
	boost::optional<MoveType> castlingIfRook = boost::none;
	if (piece->type() == TYPE_ROOK)
	{
		castlingIfRook
			= square.file() == 'a'
				? MOVE_CASTLE_QUEEN_SIDE
				: MOVE_CASTLE_KING_SIDE;
	}

	for (const Move& move : mMoves)
	{
		if (move.originSquare() == square
				|| move.finalSquare() == square
				|| (castlingIfRook && *castlingIfRook == move.moveType()))
		{
			return true;
		}
	}
	return false;
}

const std::vector<Move>& MoveHistory::getAllMoves() const
{
	return mMoves;
}
