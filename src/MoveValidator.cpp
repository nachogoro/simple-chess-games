#include "MoveValidator.h"

using namespace simplechess::details;


boost::optional<Square> MoveValidator::enPassantTarget(
		const GameStage& stage)
{
	if (!stage.move())
	{
		return {};
	}

	const PieceMove& move = stage.move()->pieceMove();

	if (move.piece() == TYPE_PAWN
			&& abs(move().dst().rank() - move.src().rank()) == 2)
	{
		return { Square::instantiateWithRankAndFile(
				((move.piece().color() == COLOR_WHITE)
				 ? 3
				 : 6),
				move.dst().file()) };
	}

	return {};
}
