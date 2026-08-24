#include "AlgebraicNotationGenerator.h"
#include <simplechess/Exceptions.h>

#include "MoveValidator.h"

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
	std::optional<Square> targetIfEnPassantCapture(
			const Board& board,
			const PieceMove& move)
	{
		if (move.piece().type() == PieceType::Pawn
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
			case CheckType::NoCheck:
				return "";
			case CheckType::Check:
				return "+";
			default:
				return "#";
		}
	}

	std::string toString(const PieceType type)
	{
		switch (type)
		{
			case PieceType::Rook:
				return "R";
			case PieceType::Knight:
				return "N";
			case PieceType::Bishop:
				return "B";
			case PieceType::Queen:
				return "Q";
			case PieceType::King:
				return "K";
			case PieceType::Pawn:
				// Nothing
				return "";
		}
		throw simplechess::InvalidArgumentException("Unknown piece type: " + std::to_string(static_cast<int>(type)));
	}

	enum class CastlingType
	{
		KingSide,
		QueenSide
	};

	std::optional<CastlingType> castlingType(const PieceMove& move)
	{
		if (move.piece().type() == PieceType::King
				&& abs(move.dst().file() - move.src().file()) == 2)
		{
			return { move.dst().file() > move.src().file()
				? CastlingType::KingSide
				: CastlingType::QueenSide };
		}

		return {};
	}

	enum AlgebraicAmbiguity
	{
		NeedsFile = 0x01,
		NeedsRank = 0x10,
	};

	/**
	 * Returns which parts of the origin square must be spelled out to tell the
	 * move apart from any other move of a piece of the same type to the same
	 * square.
	 *
	 * The file alone identifies the piece unless a candidate stands on that
	 * same file, in which case the rank does. The whole square is needed only
	 * when one candidate shares the file and a different one shares the rank.
	 */
	uint8_t getAmbiguityMask(
			const Board& board,
			const PieceMove& move)
	{
		if (castlingType(move))
		{
			// Castling can never be ambiguous
			return 0;
		}

		if (move.piece().type() == PieceType::Pawn)
		{
			// Pawns are never disambiguated this way: two pawns of the same
			// colour can only reach a common square by capturing from different
			// files, and a capture by a pawn already spells out its file.
			return 0;
		}

		const std::vector<PieceMove> allPossibleMoves
			= details::MoveValidator::allAvailableMoves(
					board,
					::targetIfEnPassantCapture(board, move),
					0, // Not castling, so irrelevant for ambiguity
					move.piece().color());

		bool isAmbiguous = false;
		bool candidateSharesFile = false;
		bool candidateSharesRank = false;

		for (const auto& otherMove : allPossibleMoves)
		{
			if (otherMove.piece().type() != move.piece().type()
					|| otherMove.dst() != move.dst()
					|| otherMove.src() == move.src())
			{
				continue;
			}

			// Another piece of the same type could move to the same square
			isAmbiguous = true;

			candidateSharesFile = candidateSharesFile
				|| otherMove.src().file() == move.src().file();
			candidateSharesRank = candidateSharesRank
				|| otherMove.src().rank() == move.src().rank();
		}

		if (!isAmbiguous)
		{
			return 0;
		}

		if (!candidateSharesFile)
		{
			return AlgebraicAmbiguity::NeedsFile;
		}

		if (!candidateSharesRank)
		{
			return AlgebraicAmbiguity::NeedsRank;
		}

		return AlgebraicAmbiguity::NeedsFile | AlgebraicAmbiguity::NeedsRank;
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
		|| (board.pieceAt(move.src())->type() == PieceType::Pawn
				&& move.dst().file() != move.src().file());

	const uint8_t ambiguityMask = ::getAmbiguityMask(
			board,
			move);

	std::ostringstream ss;
	const std::optional<CastlingType> castling
		= ::castlingType(move);

	// Castling is handled differently
	if (castling)
	{
		ss << ((*castling == CastlingType::KingSide)
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
		if ((ambiguityMask & AlgebraicAmbiguity::NeedsFile) != 0)
		{
			ss << move.src().file();
		}

		if ((ambiguityMask & AlgebraicAmbiguity::NeedsRank) != 0)
		{
			ss << static_cast<int>(move.src().rank());
		}
	}

	// 3. Add the capture symbol if appropriate
	if (isCapture) {
		if (move.piece().type() == PieceType::Pawn) {
			// All captures by a pawn include the original file even if it is
			// not ambiguous. Nothing has been written yet in this case, since
			// pawns carry no piece letter and are never disambiguated.
			ss << move.src().file();
		}
		ss << "x";
	}

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
