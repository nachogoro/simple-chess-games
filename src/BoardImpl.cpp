#include "BoardImpl.h"

#include <Exception.h>

using namespace simplechess;


BoardImpl BoardImpl::createDefaultBoard()
{
	Piece blackPawn(TYPE_PAWN, COLOR_BLACK);
// TODO
}

BoardImpl::BoardImpl(const std::map<Square, PieceImpl>& piecePositions)
	: mPiecePositions(piecePositions)
{
}

bool BoardImpl::isEmpty(const Square& square) const
{
	return mPiecePositions.count(square) == 0;
}

PieceImpl& BoardImpl::pieceAt(const Square& square)
{
	if (isEmpty(square))
	{
		throw Exception("No piece at " + square.toString());
	}

	return mPiecePositions.at(square);
}

const PieceImpl& BoardImpl::pieceAt(const Square& square) const
{
	if (isEmpty(square))
	{
		throw Exception("No piece at " + square.toString());
	}

	return mPiecePositions.at(square);
}

Board BoardImpl::toPublicBoard() const
{
	std::map<Square, Piece> result;
	for (const auto& entry : mPiecePositions)
	{
		result[entry.first] = entry.second.toPublicPiece();
	}
	return Board(result);
}

bool BoardImpl::isOccupiableBy(const Square& dstSquare, PieceColor color) const
{
	// A piece of a given color can move into a square if it is free or
	// occupied by a piece of the other color
	return mPiecePositions.count(dstSquare) == 0
		|| mPiecePositions.at(dstSquare).color() != color;
}

bool BoardImpl::isOccupiedByPieceOfColor(
		const Square& dstSquare, PieceColor color) const
{
	return mPiecePositions.count(dstSquare) != 0
		&& mPiecePositions.at(dstSquare).color() == color;
}

bool BoardImpl::isInDiagonal(const Square& src, const Square& dst) const
{
	return src != dst
		&& std::abs(src.file() - dst.file()) == std::abs(src.rank() - dst.rank());
}

bool BoardImpl::isInSameRankOrFile(const Square& src, const Square& dst) const
{
	return src != dst
		&& ((src.file() == dst.file()) || (src.rank() == dst.rank()));
}

bool BoardImpl::isReachable(const Square& src, const Square& dst) const
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
			if (!isEmpty(Square::instantiateWithRankAndFile(src.rank(), file)))
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
			if (!isEmpty(Square::instantiateWithRankAndFile(rank, src.file())))
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
			if (!isEmpty(Square::instantiateWithRankAndFile(rank, file)))
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

const Square& BoardImpl::kingSquare(PieceColor color) const
{
	for (const auto& entry : mPiecePositions)
	{
		if (entry.second.type() == TYPE_KING
				&& entry.second.color() == color)
		{
			return entry.first;
		}
	}

	throw Exception("At least one king is missing from the board!");
}
