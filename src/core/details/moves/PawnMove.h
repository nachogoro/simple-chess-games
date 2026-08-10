#ifndef PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275
#define PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275

#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/Square.h>

#include <optional>

#include <set>

namespace simplechess
{
	namespace details
	{
		std::set<PieceMove> pawnMovesUnfiltered(
				const Board& board,
				const std::optional<Square>& enPassantTarget,
				Color color,
				const Square& square);

		/**
		 * \brief Returns the squares a pawn of \p color standing on \p square
		 * attacks.
		 *
		 * A pawn is the one piece whose moves do not describe what it
		 * attacks: it advances straight ahead, which threatens nothing, and
		 * captures diagonally, which it can only be seen to do when there is
		 * something there to capture. The squares are therefore reported
		 * regardless of what stands on them, if anything.
		 */
		std::set<PieceMove> pawnAttacksUnfiltered(
				Color color,
				const Square& square);
	}
}

#endif
