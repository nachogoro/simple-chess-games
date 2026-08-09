#include "QueenMove.h"

#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

void simplechess::details::appendQueenMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Color color,
		const Square& square)
{
	// A queen moves like a rook and like a bishop combined, so it simply
	// travels in all eight directions.
	const Piece queen = {PieceType::Queen, color};

	for (const int8_t rankStep : {-1, 0, 1})
	{
		for (const int8_t fileStep : {-1, 0, 1})
		{
			if (rankStep == 0 && fileStep == 0)
			{
				continue;
			}

			BoardAnalyzer::appendMovesInDirection(
					moves, board, queen, square, rankStep, fileStep);
		}
	}
}
