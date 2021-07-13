#include <Board.h>
#include <details/BoardAnalyzer.h>

using namespace simplechess;

Board::Board(const std::map<Square, Piece>& piecePositions)
	: mPiecePositions(piecePositions)
{
}

boost::optional<Piece> Board::pieceAt(const Square& square) const
{
	const std::map<Square, Piece>::const_iterator it
		= mPiecePositions.find(square);
	if (it == mPiecePositions.end())
	{
		return boost::none;
	}
	else
	{
		return {it->second};
	}
}

const std::map<Square, Piece>& Board::occupiedSquares() const
{
	return mPiecePositions;
}
