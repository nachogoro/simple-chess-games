#ifndef PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275
#define PAWN_MOVE_H_A14504AA_BDDE_480D_9BB5_BDBD7BA02275

#include <Board.h>
#include <PieceMove.h>
#include <Square.h>

#include <boost/optional.hpp>

#include <set>

namespace simplechess
{
	namespace details
	{
		// TODO make attacking-only version (no forward moves)
		std::set<PieceMove> pawnMovesUnfiltered(
				const Board& board,
				const boost::optional<Square>& enPassantTarget,
				Color color,
				const Square& square);
	}
}

#endif
