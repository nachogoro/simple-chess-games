#include "BishopMove.h"

#include <details/BoardAnalyzer.h>

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::bishopMovesUnfiltered(
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece bishop = {TYPE_BISHOP, color};

	std::set<Square> possibleSquares;

	{
		const std::set<Square> kingBlackSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, 1, 1);

		possibleSquares.insert(
				kingBlackSide.begin(),
				kingBlackSide.end());
	}

	{
		const std::set<Square> queenBlackSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, 1, -1);

		possibleSquares.insert(
				queenBlackSide.begin(),
				queenBlackSide.end());
	}

	{
		const std::set<Square> kingWhiteSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, -1, 1);

		possibleSquares.insert(
				kingWhiteSide.begin(),
				kingWhiteSide.end());
	}

	{
		const std::set<Square> queenWhiteSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, -1, -1);

		possibleSquares.insert(
				queenWhiteSide.begin(),
				queenWhiteSide.end());
	}

	std::set<PieceMove> result;
	for (const Square& dst : possibleSquares)
	{
		result.insert(PieceMove::regularMove(
					bishop,
					square,
					dst));
	}

	return result;
}
