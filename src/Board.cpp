#include <Board.h>

using namespace simplechess;

Board::Board(const std::map<Square, Piece>& piecePositions)
	: mPiecePositions(piecePositions)
{
}

boost::optional<Piece> Board::pieceAt(const Square& square) const
{
	if (mPiecePositions.count(square) == 0)
	{
		return boost::none;
	}
	else
	{
		return mPiecePositions.at(square);
	}
}
