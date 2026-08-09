#include "BoardAnalyzer.h"

#include "../Builders.h"
#include "BoardAccess.h"
#include "MoveValidator.h"

#include <cpp/simplechess/Exceptions.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <stdexcept>

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
		= MoveValidator::allPotentiallyCapturingMovesUnfiltered(
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
			const Square dst = Square::fromRankAndFile(
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

Square BoardAnalyzer::kingSquare(const Board& board, Color color)
{
	const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

	for (uint8_t index = 0; index < 64; ++index)
	{
		const uint8_t code = squares[index];

		if (BoardAccess::isColor(code, color)
				&& BoardAccess::typeOf(code) == PieceType::King)
		{
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
