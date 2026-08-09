#include "BishopMove.h"

#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

void simplechess::details::appendBishopMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece bishop = {PieceType::Bishop, color};

	BoardAnalyzer::appendMovesInDirection(moves, board, bishop, square, 1, 1);
	BoardAnalyzer::appendMovesInDirection(moves, board, bishop, square, 1, -1);
	BoardAnalyzer::appendMovesInDirection(moves, board, bishop, square, -1, 1);
	BoardAnalyzer::appendMovesInDirection(moves, board, bishop, square, -1, -1);
}
