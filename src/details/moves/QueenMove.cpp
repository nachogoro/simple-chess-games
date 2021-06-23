#include "QueenMove.h"

#include "RookMove.h"
#include "BishopMove.h"

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::validQueenMoves(
		const Board& board,
		const Color color,
		const Square& square)
{
	// These moves assume the moving piece is a rook/bishop!
	const std::set<PieceMove> asRook = validRookMoves(board, color, square);
	const std::set<PieceMove> asBishop = validBishopMoves(board, color, square);

	std::set<PieceMove> combined;
	combined.insert(asRook.begin(), asRook.end());
	combined.insert(asBishop.begin(), asBishop.end());

	std::set<PieceMove> result;
	for (const auto& move : combined)
	{
		result.insert(PieceMove::regularMove(
					{TYPE_QUEEN, color},
					move.src(),
					move.dst()));
	}

	return result;
}
