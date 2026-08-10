#include <simplechess/Board.h>

#include "details/BoardAccess.h"

using namespace simplechess;

namespace
{
	/**
	 * Pieces are stored one byte per square. 0 is reserved for "empty", so
	 * the encoding of a piece is one-based.
	 */
	uint8_t encode(const Piece& piece)
	{
		return static_cast<uint8_t>(
				1
				+ 2 * static_cast<uint8_t>(piece.type())
				+ ((piece.color() == Color::Black) ? 1 : 0));
	}

	Piece decode(const uint8_t code)
	{
		const uint8_t zeroBased = static_cast<uint8_t>(code - 1);

		return Piece(
				static_cast<PieceType>(zeroBased / 2),
				((zeroBased % 2) == 0) ? Color::White : Color::Black);
	}
}

uint8_t BoardAccess::encodePiece(const Piece& piece)
{
	return ::encode(piece);
}

Piece BoardAccess::decodePiece(const uint8_t code)
{
	return ::decode(code);
}

const std::array<uint8_t, 64>& BoardAccess::squares(const Board& board)
{
	return board.mSquares;
}

std::array<uint8_t, 64>& BoardAccess::mutableSquares(Board& board)
{
	return board.mSquares;
}

uint8_t BoardAccess::kingSquareHint(const Board& board, const Color color)
{
	return board.mKingSquareHint[(color == Color::White) ? 0 : 1];
}

void BoardAccess::setKingSquareHint(
		const Board& board, const Color color, const uint8_t index)
{
	board.mKingSquareHint[(color == Color::White) ? 0 : 1] = index;
}

Board::Board(const std::map<Square, Piece>& piecePositions)
	: mSquares()
{
	mSquares.fill(0);
	mKingSquareHint.fill(BoardAccess::UNKNOWN);

	for (const auto& entry : piecePositions)
	{
		mSquares[BoardAccess::indexOf(entry.first)]
			= ::encode(entry.second);
	}
}

Board::Board(const std::array<uint8_t, 64>& squares)
	: mSquares(squares)
{
	mKingSquareHint.fill(BoardAccess::UNKNOWN);
}

// The lazily built map is deliberately not carried over by any of these: a
// copy is usually an intermediate position which will never be asked for it,
// and rebuilding it costs no more than copying it would.
Board::Board(const Board& other)
	: mSquares(other.mSquares),
	  mKingSquareHint(other.mKingSquareHint)
{
}

Board::Board(Board&& other) noexcept
	: mSquares(other.mSquares),
	  mKingSquareHint(other.mKingSquareHint)
{
}

Board& Board::operator=(const Board& other)
{
	if (this != &other)
	{
		mSquares = other.mSquares;
		mKingSquareHint = other.mKingSquareHint;
		mOccupiedSquares.reset();
	}

	return *this;
}

Board& Board::operator=(Board&& other) noexcept
{
	if (this != &other)
	{
		mSquares = other.mSquares;
		mKingSquareHint = other.mKingSquareHint;
		mOccupiedSquares.reset();
	}

	return *this;
}

Board::~Board() = default;

std::optional<Piece> Board::pieceAt(const Square& square) const
{
	const uint8_t code = mSquares[BoardAccess::indexOf(square)];

	if (code == 0)
	{
		return std::nullopt;
	}

	return {::decode(code)};
}

const std::map<Square, Piece>& Board::occupiedSquares() const
{
	if (!mOccupiedSquares)
	{
		std::map<Square, Piece> built;

		for (uint8_t index = 0; index < 64; ++index)
		{
			if (mSquares[index] != 0)
			{
				built.emplace(
						BoardAccess::squareOf(index),
						::decode(mSquares[index]));
			}
		}

		mOccupiedSquares = std::move(built);
	}

	return *mOccupiedSquares;
}
