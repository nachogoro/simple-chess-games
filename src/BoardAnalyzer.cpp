#include "BoardAnalyzer.h"

#include <Exception.h>

#include <algorithm>
#include <cstdlib>

using namespace simplechess;

bool BoardAnalyzer::isSquareThreatenedBy(
		const Board& board,
		const Square& square,
		PieceColor color)
{
	//TODO
	return true;
}

bool BoardAnalyzer::isEmpty(const Board& board, const Square& square)
{
	return !board.pieceAt(square);
}

bool BoardAnalyzer::isOccupiableBy(
		const Board& board, const Square& dstSquare, PieceColor color)
{
	// A piece of a given color can move into a square if it is free or
	// occupied by a piece of the other color
	return isEmpty(board, dstSquare)
		|| board.pieceAt(dstSquare)->color() != color;
}

bool BoardAnalyzer::isOccupiedByPieceOfColor(
		const Board& board, const Square& dstSquare, PieceColor color)
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
		throw Exception("A square can always reach itself!");
	}

	if (src.rank() == dst.rank())
	{
		// They are in the same rank, so they might be connected horizontally
		const char originFile = std::min(src.file(), dst.file());
		const char destFile = std::max(src.file(), dst.file());

		for (char file = originFile + 1; file != destFile; ++file)
		{
			if (!isEmpty(board,
						Square::instantiateWithRankAndFile(src.rank(), file)))
			{
				return false;
			}
		}

		return true;
	}

	if (src.file() == dst.file())
	{
		// They are in the same file, so they might be connected vertically
		const uint8_t originRank = std::min(src.rank(), dst.rank());
		const uint8_t destRank = std::max(src.rank(), dst.rank());

		for (uint8_t rank = originRank + 1; rank != destRank; ++rank)
		{
			if (!isEmpty(board,
						Square::instantiateWithRankAndFile(rank, src.file())))
			{
				return false;
			}
		}

		return true;
	}

	if (isInDiagonal(src, dst))
	{
		// They are in the same diagonal.
		// Iterate over all squares between src and dst
		const uint8_t rankStep = dst.rank() - src.rank() > 0 ? 1 : -1;
		const uint8_t fileStep = dst.file() - src.file() > 0 ? 1 : -1;

		const uint8_t originRank = src.rank() + rankStep;
		const char originFile = src.file() + fileStep;

		uint8_t rank = originRank;
		char file = originFile;

		while (rank != dst.rank() && file != dst.file())
		{
			if (!isEmpty(board,
						Square::instantiateWithRankAndFile(rank, file)))
			{
				return false;
			}
			rank += rankStep;
			file += fileStep;
		}

		return true;
	}
	return false;
}

const Square& BoardAnalyzer::kingSquare(const Board& board, PieceColor color)
{
	for (const auto& entry : board.mPiecePositions)
	{
		if (entry.second.type() == TYPE_KING
				&& entry.second.color() == color)
		{
			return entry.first;
		}
	}

	throw Exception("At least one king is missing from the board!");
}
