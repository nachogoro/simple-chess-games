#include "FenUtils.h"

#include <stdexcept>

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
		= sPieceMap.by<Piece>().find(piece);

	if (it == sPieceMap.right.end())
	{
		throw std::invalid_argument("Cannot convert piece to FEN");
	}

	return it->second;
}

char FenUtils::stringToPiece(const char c)
{
	boost::bimap<char, Piece>::left_const_iterator it
		= sPieceMap.by<char>().find(c);

	if (it == sPieceMap.left.end())
	{
		throw std::invalid_argument(
				"Character '" + c
				+ "' is not a piece-representing character in FEN notation");
	}

	return it->second;
}
