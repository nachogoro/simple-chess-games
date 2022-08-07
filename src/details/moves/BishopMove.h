#ifndef BISHOP_MOVE_H_605B33E7_154C_4435_9C9C_B8F92137737F
#define BISHOP_MOVE_H_605B33E7_154C_4435_9C9C_B8F92137737F

#include <simplechess/Board.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> bishopMovesUnfiltered(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
