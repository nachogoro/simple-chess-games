#ifndef QUEEN_MOVE_H_8F9E4B75_E37B_4187_A4F9_7B039706AF51
#define QUEEN_MOVE_H_8F9E4B75_E37B_4187_A4F9_7B039706AF51

#include <Board.h>
#include <PieceMove.h>
#include <Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> validQueenMoves(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
