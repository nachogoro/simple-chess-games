#include "RookMove.h"

#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

void simplechess::details::appendRookMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece rook = {PieceType::Rook, color};

	BoardAnalyzer::appendMovesInDirection(moves, board, rook, square, 0, 1);
	BoardAnalyzer::appendMovesInDirection(moves, board, rook, square, 0, -1);
	BoardAnalyzer::appendMovesInDirection(moves, board, rook, square, 1, 0);
	BoardAnalyzer::appendMovesInDirection(moves, board, rook, square, -1, 0);
}
