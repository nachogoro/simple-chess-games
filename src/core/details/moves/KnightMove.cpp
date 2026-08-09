#include "KnightMove.h"

#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

void simplechess::details::appendKnightMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece knight = {PieceType::Knight, color};

	static const int8_t offsets[8][2] = {
		{1, 2}, {2, 1}, {2, -1}, {1, -2},
		{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

	for (const auto& offset : offsets)
	{
		const int rank = square.rank() + offset[0];
		const int file = square.file() + offset[1];

		if (!Square::isInsideBoundaries(
					static_cast<uint8_t>(rank),
					static_cast<char>(file)))
		{
			continue;
		}

		const Square dst = Square::fromRankAndFile(
				static_cast<uint8_t>(rank),
				static_cast<char>(file));

		if (BoardAnalyzer::isOccupiableBy(board, dst, color))
		{
			moves.push_back(PieceMove::regularMove(knight, square, dst));
		}
	}
}
