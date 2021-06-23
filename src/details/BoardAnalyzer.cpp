#include "BoardAnalyzer.h"

#include <Exceptions.h>
#include <details/MoveValidator.h>

#include <algorithm>
#include <cstdlib>

using namespace simplechess;
using namespace simplechess::details;

bool BoardAnalyzer::isSquareThreatenedBy(
		const Board& board,
		const Square& square,
		Color color)
{
	if (!isOccupiableBy(board, square, color))
	{
		return false;
	}

	const std::set<PieceMove> availableMoves
		= MoveValidator::allAvailableMovesUnfiltered(
				board,
				{}, // En passant is irrelevant for this
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

bool BoardAnalyzer::isInCheck(
		const Board& board,
		const Color color)
{
	return isSquareThreatenedBy(
			board,
			kingSquare(board, color),
			oppositeColor(color));
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
	return !isEmpty(board, dstSquare)
		&& board.pieceAt(dstSquare)->color() == color;
}

std::set<Square> BoardAnalyzer::reachableSquaresInDirection(
		const Board& board,
		const Square& src,
		const Color color,
		const int8_t rankStep,
		const int8_t fileStep)
{
	std::set<Square> result;

	// There are at most 7 squares left in any direction
	for (int8_t i = 1; i < 8; ++i)
	{
		if (Square::isInsideBoundaries(
					src.rank() + i*rankStep,
					src.file() + i*fileStep))
		{
			const Square dst = Square::instantiateWithRankAndFile(
					src.rank() + i*rankStep,
					src.file() + i*fileStep);

			if (!isOccupiableBy(board, dst, color))
			{
				// First non-ocuppiable square
				break;
			}

			result.insert(dst);

			if (!BoardAnalyzer::isEmpty(board, dst))
			{
				// First occupied square, we can't keep moving in this
				// direction
				break;
			}
		}
		else
		{
			// End of the board
			break;
		}
	}

	return result;
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
