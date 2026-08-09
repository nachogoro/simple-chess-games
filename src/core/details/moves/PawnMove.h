#ifndef PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275
#define PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275

#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/Square.h>

#include <optional>

#include <vector>

namespace simplechess
{
	namespace details
	{
		void appendPawnMovesUnfiltered(
				std::vector<PieceMove>& moves,
				const Board& board,
				const std::optional<Square>& enPassantTarget,
				Color color,
				const Square& square);
	}
}

#endif
