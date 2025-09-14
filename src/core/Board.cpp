#include <cpp/simplechess/Board.h>
#include "details/BoardAnalyzer.h"

using namespace simplechess;

Board::Board(const std::map<Square, Piece>& piecePositions)
	: mPiecePositions(piecePositions)
{
}

std::optional<Piece> Board::pieceAt(const Square& square) const
{
	const std::map<Square, Piece>::const_iterator it
		= mPiecePositions.find(square);
	if (it == mPiecePositions.end())
	{
		return std::nullopt;
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
