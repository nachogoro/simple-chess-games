#include <Board.h>
#include <details/BoardAnalyzer.h>

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

const std::map<Square, Piece>& Board::occupiedSquares() const
{
	return mPiecePositions;
}

Board Board::makeMove(const PieceMove& move) const
{
	std::map<Square, Piece> positions = occupiedSquares();

	if (move.piece().type() == TYPE_KING
			&& abs(move.dst().file() - move.src().file()) == 2)
	{
		// Castling
		// Move the king...
		positions.insert({move.dst(), move.piece()});
		positions.erase(move.src());

		// ... and the rook
		const Square rookSrc = (move.dst().file() == 'g')
			? Square::instantiateWithRankAndFile(move.dst().rank(), 'h')
			: Square::instantiateWithRankAndFile(move.dst().rank(), 'a');

		const Square rookDst = (move.dst().file() == 'g')
			? Square::instantiateWithRankAndFile(move.dst().rank(), 'f')
			: Square::instantiateWithRankAndFile(move.dst().rank(), 'd');

		positions.insert({rookDst, positions.at(rookSrc)});
		positions.erase(rookSrc);

		return positions;
	}

	if (move.piece().type() == TYPE_PAWN
			&& move.src().file() != move.dst().file()
			&& details::BoardAnalyzer::isEmpty(*this, move.dst()))
	{
		// En passant
		positions.insert({move.dst(), move.piece()});
		positions.erase(move.src());

		// Remove the captured pawn
		positions.erase(
				Square::instantiateWithRankAndFile(
					move.dst().rank() + (move.dst().rank() == 6 ? -1 : 1),
					move.dst().file()));

		return positions;
	}

	positions.insert(
			{move.dst(),
			move.promoted()
				? Piece(*move.promoted(), move.piece().color())
				: move.piece()});

	positions.erase(move.src());
	return positions;
}
