#ifndef ROOK_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0
#define ROOK_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0

#include <Board.h>
#include <PieceMove.h>
#include <Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> rookMovesUnfiltered(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
