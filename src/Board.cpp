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

std::vector<Square> occupiedSquares() const
{
	std::vector<Square> result;
	for (const auto& kv : mPiecePositions) {
		// Keys in std::map are sorted by their less-than operator. Since the
		// less-than operator of Square follows the same criterion we want
		// here, we can simply fill in the vector and it will be sorted.
		result.push_back(kv.first);
	}

	return result;
}
