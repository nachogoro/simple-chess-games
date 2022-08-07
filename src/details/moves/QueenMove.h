#ifndef QUEEN_MOVE_H_8F9E4B75_E37B_4187_A4F9_7B039706AF51
#define QUEEN_MOVE_H_8F9E4B75_E37B_4187_A4F9_7B039706AF51

#include <simplechess/Board.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> queenMovesUnfiltered(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
