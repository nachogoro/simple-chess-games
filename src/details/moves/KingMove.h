#ifndef KING_MOVE_H_3C6B8719_3428_462A_9B07_106DEC6038D8
#define KING_MOVE_H_3C6B8719_3428_462A_9B07_106DEC6038D8

#include <Board.h>
#include <PieceMove.h>
#include <Square.h>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> validKingMoves(
				const Board& board,
				uint8_t castlingRights,
				Color color,
				const Square& square);

		std::set<PieceMove> kingMovesExceptCastling(
				const Board& board,
				Color color,
				const Square& square);
	}
}

#endif
