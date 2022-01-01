#include "FenUtils.h"

#include <GameStage.h>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <stdexcept>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	template <typename L, typename R>
		boost::bimap<L, R>
		makeBimap(std::initializer_list<typename boost::bimap<L, R>::value_type> list)
		{
			return boost::bimap<L, R>(list.begin(), list.end());
		}
}

boost::bimap<char, Piece> FenUtils::sPieceMap
	= internal::makeBimap<char, Piece>(
			{
				{'P', {TYPE_PAWN, COLOR_WHITE}},
				{'R', {TYPE_ROOK, COLOR_WHITE}},
				{'N', {TYPE_KNIGHT, COLOR_WHITE}},
				{'B', {TYPE_BISHOP, COLOR_WHITE}},
				{'Q', {TYPE_QUEEN, COLOR_WHITE}},
				{'K', {TYPE_KING, COLOR_WHITE}},
				{'p', {TYPE_PAWN, COLOR_BLACK}},
				{'r', {TYPE_ROOK, COLOR_BLACK}},
				{'n', {TYPE_KNIGHT, COLOR_BLACK}},
				{'b', {TYPE_BISHOP, COLOR_BLACK}},
				{'q', {TYPE_QUEEN, COLOR_BLACK}},
				{'k', {TYPE_KING, COLOR_BLACK}},
			});

char FenUtils::pieceToString(const Piece& piece)
{
	boost::bimap<char, Piece>::right_const_iterator it
		= sPieceMap.right.find(piece);

	if (it == sPieceMap.right.end())
	{
		throw std::invalid_argument("Cannot convert piece to FEN");
	}

	return it->second;
}

Piece FenUtils::stringToPiece(const char c)
{
	boost::bimap<char, Piece>::left_const_iterator it
		= sPieceMap.left.find(c);

	if (it == sPieceMap.left.end())
	{
		throw std::invalid_argument(std::string("Character \'")
				+ c
				+ "\' is not a piece-representing character in FEN notation");
	}

	return it->second;
}

std::string FenUtils::fenForRepetitions(const std::string& fen)
{
		std::vector<std::string> fenTokens;
		boost::split(fenTokens, fen,  [](char c) { return c == ' ';});
		fenTokens.pop_back();
		fenTokens.pop_back();
		return boost::algorithm::join(fenTokens, " ");
}

std::string FenUtils::generateFen(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const boost::optional<Square>& epTarget,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock)
{
	// A FEN string is an ASCII string composed of six fields, separated from
	// each other by a space.

	std::stringstream ss;

	// From Wikipedia:
	// 1. Piece placement (from White's perspective). Each rank is described,
	// starting with rank 8 and ending with rank 1; within each rank, the
	// contents of each square are described from file "a" through file "h".
	// Following the Standard Algebraic Notation (SAN), each piece is
	// identified by a single letter taken from the standard English names
	// (pawn = "P", knight = "N", bishop = "B", rook = "R", queen = "Q" and
	// king = "K"). White pieces are designated using upper-case letters
	// ("PNBRQK") while black pieces use lowercase ("pnbrqk"). Empty squares
	// are noted using digits 1 through 8 (the number of empty squares), and
	// "/" separates ranks.

	uint8_t emptySquaresRun = 0;

	for (uint8_t rank = 8; rank > 0; --rank)
	{
		for (char file = 'a'; file <= 'h'; ++file)
		{
			const Square square = Square::fromRankAndFile(rank, file);

			const boost::optional<Piece> piece = board.pieceAt(square);

			if (!piece)
			{
				emptySquaresRun++;
				continue;
			}

			// We have encountered a piece, list number of empty squares if any
			if (emptySquaresRun != 0)
			{
				ss << static_cast<int>(emptySquaresRun);
				emptySquaresRun = 0;
			}

			ss << pieceToString(*piece);
		}

		// We have finished a rank, list number of empty squares if any
		if (emptySquaresRun != 0)
		{
			ss << static_cast<int>(emptySquaresRun);
			emptySquaresRun = 0;
		}

		if (rank != 1)
		{
			ss << "/";
		}
	}

	// 2. Active color. "w" means White moves next, "b" means Black moves next.
	ss << " ";
	ss << ((activeColor == COLOR_WHITE)
		? "w"
		: "b");

	// 3. Castling availability. If neither side can castle, this is "-".
	// Otherwise, this has one or more letters: "K" (White can castle
	// kingside), "Q" (White can castle queenside), "k" (Black can castle
	// kingside), and/or "q" (Black can castle queenside). A move that
	// temporarily prevents castling does not negate this notation.
	ss << " ";
	if (castlingRights == 0)
	{
		ss << "-";
	}
	else
	{
		if ((castlingRights & CASTLING_RIGHT_WHITE_KINGSIDE) != 0)
		{
			ss << "K";
		}

		if ((castlingRights & CASTLING_RIGHT_WHITE_QUEENSIDE) != 0)
		{
			ss << "Q";
		}

		if ((castlingRights & CASTLING_RIGHT_BLACK_KINGSIDE) != 0)
		{
			ss << "k";
		}

		if ((castlingRights & CASTLING_RIGHT_BLACK_QUEENSIDE) != 0)
		{
			ss << "q";
		}
	}
	// 4. En passant target square in algebraic notation. If there's no en
	// passant target square, this is "-". If a pawn has just made a two-square
	// move, this is the position "behind" the pawn. This is recorded
	// regardless of whether there is a pawn in position to make an en passant
	// capture.
	ss << " ";
	if (epTarget)
	{
		ss << epTarget->toString();
	}
	else
	{
		ss << "-";
	}

	// 5. Halfmove clock: The number of halfmoves since the last capture or
	// pawn advance, used for the fifty-move rule.
	ss << " " << halfmoveClock;

	// 6. Fullmove number: The number of the full move. It starts at 1, and is
	// incremented after Black's move.
	ss << " " << fullmoveClock;

	return ss.str();
}
