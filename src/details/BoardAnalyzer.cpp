#include "BoardAnalyzer.h"

#include <Exceptions.h>

#include <algorithm>
#include <cstdlib>

using namespace simplechess;

bool BoardAnalyzer::isSquareThreatenedBy(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		uint8_t castlingRights,
		const Square& square,
		Color color)
{
	if (!isOccupiableBy(board, square, color))
	{
		return false;
	}

	const std::set<PieceMove> availableMoves = MoveValidator::allValidMoves(
			board,
			enPassantTarget,
			castlingRights,
			color);

	for (const auto& move : availableMoves)
	{
		if (move.dst() == square)
		{
			return true;
		}
	}

	return false;
}

bool BoardAnalyzer::isEmpty(const Board& board, const Square& square)
{
	return !board.pieceAt(square);
}

bool BoardAnalyzer::isOccupiableBy(
		const Board& board, const Square& dstSquare, Color color)
{
	// A piece of a given color can move into a square if it is free or
	// occupied by a piece of the other color
	return isEmpty(board, dstSquare)
		|| board.pieceAt(dstSquare)->color() != color;
}

bool BoardAnalyzer::isOccupiedByPieceOfColor(
		const Board& board, const Square& dstSquare, Color color)
{
	return isEmpty(board, dstSquare) != 0
		&& board.pieceAt(dstSquare)->color() == color;
}

bool BoardAnalyzer::isInDiagonal(
		const Square& src, const Square& dst)
{
	return src != dst
		&& std::abs(src.file() - dst.file()) == std::abs(src.rank() - dst.rank());
}

bool BoardAnalyzer::isInSameRankOrFile(const Square& src, const Square& dst)
{
	return src != dst
		&& ((src.file() == dst.file()) || (src.rank() == dst.rank()));
}

bool BoardAnalyzer::isReachable(
		const Board& board, const Square& src, const Square& dst)
{
	if (src.rank() == dst.rank() && src.file() == dst.file()) {
		throw std::invalid_argument("A square can always reach itself!");
	}

	if (!isInDiagonal(src, dst) && !isInSameRankOrFile(src, dst))
	{
		return false;
	}

	const uint8_t rankStep =
		(src.rank() == dst.rank())
			? 0
			: (src.rank() > dst.rank())
				? -1
				: 1;

	const uint8_t fileStep =
		(src.file() == dst.file())
			? 0
			: (src.file() > dst.file())
				? -1
				: 1;

	uint8_t rank = src.rank() + rankStep;
	char file = src.file() + fileStep;

	while (rank != dst.rank() || file != dst.file())
	{
		if (!isEmpty(board, Square::instantiateWithRankAndFile(rank, file)))
		{
			return false;
		}

		rank += rankStep;
		file += fileStep;
	}

	return true;
}

const Square& BoardAnalyzer::kingSquare(const Board& board, Color color)
{
	for (const auto& entry : board.occupiedSquares())
	{
		if (entry.second.type() == TYPE_KING
				&& entry.second.color() == color)
		{
			return entry.first;
		}
	}

	throw IllegalStateException("At least one king is missing from the board!");
}
