#ifndef KNIGHT_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0
#define KNIGHT_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0

#include <simplechess/Board.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> knightMovesUnfiltered(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
