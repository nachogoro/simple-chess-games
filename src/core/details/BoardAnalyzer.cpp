#include "BoardAnalyzer.h"

#include "../Builders.h"
#include "BoardAccess.h"
#include "MoveValidator.h"

#include <simplechess/Exceptions.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <stdexcept>

using namespace simplechess;
using namespace simplechess::details;

namespace
{
	// Directions in which a rook or a queen threatens, then those of a bishop
	// or a queen, as {file, rank} steps.
	constexpr int8_t sStraightDirections[4][2]
		= {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	constexpr int8_t sDiagonalDirections[4][2]
		= {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	constexpr int8_t sKnightOffsets[8][2]
		= {{1, 2}, {2, 1}, {2, -1}, {1, -2},
		   {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

	/**
	 * Whether any piece of \p color standing on a square reachable from \p
	 * originIndex along the given directions threatens it.
	 *
	 * Walks outwards until it runs off the board or meets a piece: if that
	 * piece is a sliding piece of \p color which moves in that direction, the
	 * square is threatened.
	 */
	bool threatenedBySlider(
			const std::array<uint8_t, 64>& squares,
			const uint8_t originIndex,
			const simplechess::Color color,
			const int8_t (&directions)[4][2],
			const simplechess::PieceType slider)
	{
		using namespace simplechess;

		for (const auto& direction : directions)
		{
			for (int distance = 1; distance < 8; ++distance)
			{
				const int index = BoardAccess::offsetIndex(
						originIndex,
						direction[0] * distance,
						direction[1] * distance);

				if (index < 0)
				{
					break;
				}

				const uint8_t code = squares[static_cast<size_t>(index)];

				if (code == 0)
				{
					continue;
				}

				// The first piece in the way either threatens the square or
				// blocks everything behind it.
				if (BoardAccess::isColor(code, color))
				{
					const PieceType type = BoardAccess::typeOf(code);
					if (type == slider || type == PieceType::Queen)
					{
						return true;
					}
				}

				break;
			}
		}

		return false;
	}
}

bool BoardAnalyzer::isSquareThreatenedBy(
		const Board& board,
		const Square& square,
		Color color)
{
	if (!isOccupiableBy(board, square, color))
	{
		return false;
	}

	// Rather than generating every move the attacking side could make and
	// looking for one that lands here, radiate outwards from the square and
	// ask what would be standing where an attacker would have to stand. That
	// answers the same question by reading a few dozen squares instead of
	// building a whole set of moves, and it can stop as soon as it finds one.
	const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);
	const uint8_t origin = BoardAccess::indexOf(square);

	if (threatenedBySlider(squares, origin, color, sStraightDirections, PieceType::Rook)
			|| threatenedBySlider(squares, origin, color, sDiagonalDirections, PieceType::Bishop))
	{
		return true;
	}

	for (const auto& offset : sKnightOffsets)
	{
		const int index = BoardAccess::offsetIndex(origin, offset[0], offset[1]);

		if (index >= 0)
		{
			const uint8_t code = squares[static_cast<size_t>(index)];
			if (BoardAccess::isColor(code, color)
					&& BoardAccess::typeOf(code) == PieceType::Knight)
			{
				return true;
			}
		}
	}

	// Pawns capture forwards, so a white pawn threatening this square must be
	// on the rank below it and a black pawn on the rank above.
	const int pawnRankDelta = (color == Color::White) ? -1 : 1;
	for (const int fileDelta : {-1, 1})
	{
		const int index
			= BoardAccess::offsetIndex(origin, fileDelta, pawnRankDelta);

		if (index >= 0)
		{
			const uint8_t code = squares[static_cast<size_t>(index)];
			if (BoardAccess::isColor(code, color)
					&& BoardAccess::typeOf(code) == PieceType::Pawn)
			{
				return true;
			}
		}
	}

	for (int fileDelta = -1; fileDelta <= 1; ++fileDelta)
	{
		for (int rankDelta = -1; rankDelta <= 1; ++rankDelta)
		{
			if (fileDelta == 0 && rankDelta == 0)
			{
				continue;
			}

			const int index
				= BoardAccess::offsetIndex(origin, fileDelta, rankDelta);

			if (index >= 0)
			{
				const uint8_t code = squares[static_cast<size_t>(index)];
				if (BoardAccess::isColor(code, color)
						&& BoardAccess::typeOf(code) == PieceType::King)
				{
					return true;
				}
			}
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

void BoardAnalyzer::appendMovesInDirection(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Piece& piece,
		const Square& src,
		const int8_t rankStep,
		const int8_t fileStep)
{
	// There are at most 7 squares left in any direction
	for (int8_t i = 1; i < 8; ++i)
	{
		const int rank = src.rank() + i*rankStep;
		const int file = src.file() + i*fileStep;

		if (!Square::isInsideBoundaries(
					static_cast<uint8_t>(rank),
					static_cast<char>(file)))
		{
			// End of the board
			break;
		}

		const Square dst = Square::fromRankAndFile(
				static_cast<uint8_t>(rank),
				static_cast<char>(file));

		if (!isOccupiableBy(board, dst, piece.color()))
		{
			// First non-ocuppiable square
			break;
		}

		moves.push_back(PieceMove::regularMove(piece, src, dst));

		if (!BoardAnalyzer::isEmpty(board, dst))
		{
			// First occupied square, we can't keep moving in this
			// direction
			break;
		}
	}
}

Square BoardAnalyzer::kingSquare(const Board& board, Color color)
{
	const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

	const auto holdsKing = [&squares, color](const uint8_t index) {
		const uint8_t code = squares[index];
		return BoardAccess::isColor(code, color)
			&& BoardAccess::typeOf(code) == PieceType::King;
	};

	// Trust the hint only as far as confirming it, so a stale one costs a
	// scan rather than a wrong answer.
	const uint8_t hint = BoardAccess::kingSquareHint(board, color);

	if (hint != BoardAccess::UNKNOWN && holdsKing(hint))
	{
		return BoardAccess::squareOf(hint);
	}

	for (uint8_t index = 0; index < 64; ++index)
	{
		if (holdsKing(index))
		{
			BoardAccess::setKingSquareHint(board, color, index);
			return BoardAccess::squareOf(index);
		}
	}

	throw std::invalid_argument("At least one king is missing from the board!");
}

Board BoardAnalyzer::makeMoveOnBoard(
		const Board& board,
		const PieceMove& move)
{
	// This runs once per candidate move while generating the legal moves of a
	// position, so it works on the packed array directly: copying the board
	// is a single fixed-size copy and applying the move is a couple of byte
	// stores.
	Board result = board;
	std::array<uint8_t, 64>& squares = BoardAccess::mutableSquares(result);

	const uint8_t srcIndex = BoardAccess::indexOf(move.src());
	const uint8_t dstIndex = BoardAccess::indexOf(move.dst());

	if (move.piece().type() == PieceType::King)
	{
		BoardAccess::setKingSquareHint(
				result, move.piece().color(), dstIndex);
	}

	if (move.piece().type() == PieceType::King
			&& abs(move.dst().file() - move.src().file()) == 2)
	{
		// Castling: move the king...
		squares[dstIndex] = squares[srcIndex];
		squares[srcIndex] = 0;

		// ... and the rook
		const Square rookSrc = (move.dst().file() == 'g')
			? Square::fromRankAndFile(move.dst().rank(), 'h')
			: Square::fromRankAndFile(move.dst().rank(), 'a');

		const Square rookDst = (move.dst().file() == 'g')
			? Square::fromRankAndFile(move.dst().rank(), 'f')
			: Square::fromRankAndFile(move.dst().rank(), 'd');

		squares[BoardAccess::indexOf(rookDst)]
			= squares[BoardAccess::indexOf(rookSrc)];
		squares[BoardAccess::indexOf(rookSrc)] = 0;

		return result;
	}

	if (move.piece().type() == PieceType::Pawn
			&& move.src().file() != move.dst().file()
			&& squares[dstIndex] == 0)
	{
		// En passant
		squares[dstIndex] = squares[srcIndex];
		squares[srcIndex] = 0;

		// Remove the captured pawn, which stands beside the moving pawn's
		// origin rather than on its destination square
		squares[BoardAccess::indexOf(
				Square::fromRankAndFile(
					move.dst().rank() + (move.dst().rank() == 6 ? -1 : 1),
					move.dst().file()))] = 0;

		return result;
	}

	squares[dstIndex] = move.promoted()
		? BoardAccess::encodePiece(Piece(*move.promoted(), move.piece().color()))
		: BoardAccess::encodePiece(move.piece());

	squares[srcIndex] = 0;

	return result;
}
