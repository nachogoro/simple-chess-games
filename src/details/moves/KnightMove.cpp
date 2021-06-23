#include "KnightMove.h"

#include <details/BoardAnalyzer.h>

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::validKnightMoves(
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece knight = {TYPE_KNIGHT, color};

	const std::set<int8_t> longSteps = {-2, 2};
	const std::set<int8_t> shortSteps = {-1, 1};

	std::set<PieceMove> result;

	for (const int8_t longStep : longSteps)
	{
		for (const int8_t shortStep : shortSteps)
		{
			if (Square::isInsideBoundaries(
						square.rank() + longStep,
						square.file() + shortStep))
			{
				const Square dst = Square::instantiateWithRankAndFile(
						square.rank() + longStep,
						square.file() + shortStep);

				if (BoardAnalyzer::isOccupiableBy(board, dst, color))
				{
					result.insert(PieceMove::regularMove(knight, square, dst));
				}
			}

			if (Square::isInsideBoundaries(
						square.rank() + shortStep,
						square.file() + longStep))
			{
				const Square dst = Square::instantiateWithRankAndFile(
						square.rank() + shortStep,
						square.file() + longStep);

				if (BoardAnalyzer::isOccupiableBy(board, dst, color))
				{
					result.insert(PieceMove::regularMove(knight, square, dst));
				}
			}
		}
	}

	return result;
}
