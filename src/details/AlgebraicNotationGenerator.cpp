#include "AlgebraicNotationGenerator.h"

#include "details/MoveValidator.h"

#include <set>
#include <sstream>

using namespace simplechess;
using namespace simplechess::details;

namespace
{
	/**
	 * Infers the en passant target square (the one behind the pawn) in a given
	 * position from the move made by the opposite side, for the purpose of
	 * ambiguity detection.
	 * Basically, if move is an en passant capture then we can be sure that the
	 * landing square was an en passant target. If not, we can't know if any
	 * other target was, but we don't care.
	 */
	boost::optional<Square> targetIfEnPassantCapture(
			const Board& board,
			const PieceMove& move)
	{
		if (move.piece().type() == TYPE_PAWN
				&& move.src().file() != move.dst().file()
				&& !board.pieceAt(move.dst()))
		{
			// A pawn has moved diagonally but the landing square was empty, it
			// must be en passant
			return { move.dst() };
		}
		return {};
	}

	std::string toString(const CheckType checkType)
	{
		switch (checkType)
		{
			case NO_CHECK:
				return "";
			case CHECK:
				return "+";
			default:
				return "#";
		}
	}

	std::string toString(const PieceType type)
	{
		switch (type)
		{
			case TYPE_ROOK:
				return "R";
			case TYPE_KNIGHT:
				return "N";
			case TYPE_BISHOP:
				return "B";
			case TYPE_QUEEN:
				return "Q";
			case TYPE_KING:
				return "K";
			case TYPE_PAWN:
				// Nothing
				return "";
		}
		throw std::invalid_argument("Unknown piece type: " + type);
	}

	enum CastlingType
	{
		CASTLING_KINGSIDE,
		CASTLING_QUEENSIDE
	};

	boost::optional<CastlingType> castlingType(const PieceMove& move)
	{
		if (move.piece().type() == TYPE_KING
				&& abs(move.dst().file() - move.src().file()) == 2)
		{
			return { move.dst().file() > move.src().file()
				? CASTLING_KINGSIDE
					: CASTLING_QUEENSIDE };
		}

		return {};
	}

	enum AlgebraicAmbiguity
	{
		SAME_RANK = 0x01,
		SAME_FILE = 0x10,
	};

	uint8_t getAmbiguityMask(
			const Board& board,
			const PieceMove& move)
	{
		if (castlingType(move))
		{
			// Castling can never be ambiguous
			return 0;
		}

		const std::set<PieceMove> allPossibleMoves
			= details::MoveValidator::allAvailableMoves(
					board,
					::targetIfEnPassantCapture(board, move),
					0, // Not castling, so irrelevant for ambiguity
					move.piece().color());

		uint8_t ambiguityMask = 0;

		for (const auto& otherMove : allPossibleMoves)
		{
			if (otherMove.piece().type() == move.piece().type()
					&& otherMove.dst() == move.dst()
					&& otherMove.src() != move.src())
			{
				// If another piece of the same type could move to the same
				// square, the move is ambiguous
				if (otherMove.src().rank() == move.src().rank())
				{
					ambiguityMask |= SAME_RANK;
				}

				if (otherMove.src().file() == move.src().file())
				{
					ambiguityMask |= SAME_FILE;
				}
			}
		}

		return ambiguityMask;
	}
}

std::string AlgebraicNotationGenerator::toAlgebraicNotation(
		const Board& board,
		const PieceMove& move,
		const bool drawOffered,
		const CheckType checkType)
{
	// It's a capture if there was an enemy piece on the landing square or
	// it was an en passant capture
	const bool isCapture =
		board.pieceAt(move.dst()).has_value()
		|| (board.pieceAt(move.src())->type() == TYPE_PAWN
				&& move.dst().file() != move.src().file());

	const uint8_t ambiguityMask = ::getAmbiguityMask(
			board,
			move);

	std::stringstream ss;
	const boost::optional<CastlingType> castling
		= ::castlingType(move);

	// Castling is handled differently
	if (castling)
	{
		ss << ((*castling == CASTLING_KINGSIDE)
				? "O-O"
				: "O-O-O");

		ss << ::toString(checkType);
		ss << (drawOffered ? "(=)" : "");

		return ss.str();
	}

	// 1. First add the piece letter
	ss << ::toString(move.piece().type());

	// 2. Add the disambiguation characters if needed
	if (ambiguityMask != 0)
	{
		if ((ambiguityMask & SAME_RANK) != 0)
		{
			// It shares rank with another piece causing ambiguity, the
			// file is needed
			ss << move.src().file();
		}

		if ((ambiguityMask & SAME_FILE) != 0)
		{
			// It shares file with another piece causing ambiguity, the
			// rank is needed
			ss << static_cast<int>(move.src().rank());
		}
	}

	// 3. Add the capture symbol if appropriate
	ss << (isCapture ? "x" : "");

	// 4. Add destination square
	ss << move.dst().toString();

	// 5. If pawn promotion, add promoted piece type
	if (move.promoted())
	{
		ss << "=" << ::toString(*move.promoted());
	}

	// 6. Add check
	ss << ::toString(checkType);

	// 7. Add draw offer
	ss << (drawOffered ? "(=)" : "");

	return ss.str();
}
