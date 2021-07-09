#include "QueenMove.h"

#include "RookMove.h"
#include "BishopMove.h"

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> validQueenMoves(
		const Board& board,
		const Color color,
		const Square& square)
{
	const std::set<PieceMove> asRook = validRookMoves(board, color, square);
	const std::set<PieceMove> asBishop = validBishopMoves(board, color, square);

	std::set<PieceMove> combined;
	combined.insert(asRook.begin(), asRook.end());
	combined.insert(asBishop.begin(), asBishop.end());

	return combined;
}
