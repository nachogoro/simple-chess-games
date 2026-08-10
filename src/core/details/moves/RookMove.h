#ifndef ROOK_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0
#define ROOK_MOVE_H_268B26F2_2BFF_496B_84CE_B8A5AEE00FE0

#include <simplechess/Board.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <vector>

namespace simplechess
{
	namespace details
	{
		void appendRookMovesUnfiltered(
				std::vector<PieceMove>& moves,
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
