#include "RookMove.h"

#include <details/BoardAnalyzer.h>

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::validRookMoves(
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece rook = {TYPE_ROOK, color};

	std::set<Square> possibleSquares;

	{
		const std::set<Square> horizontalKingSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, 0, 1);

		possibleSquares.insert(
				horizontalKingSide.begin(),
				horizontalKingSide.end());
	}

	{
		const std::set<Square> horizontalQueenSide
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, 0, -1);

		possibleSquares.insert(
				horizontalQueenSide.begin(),
				horizontalQueenSide.end());
	}

	{
		const std::set<Square> verticalTowardsBlack
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, 1, 0);

		possibleSquares.insert(
				verticalTowardsBlack.begin(),
				verticalTowardsBlack.end());
	}

	{
		const std::set<Square> verticalTowardsWhite
			= BoardAnalyzer::reachableSquaresInDirection(
					board, square, color, -1, 0);

		possibleSquares.insert(
				verticalTowardsWhite.begin(),
				verticalTowardsWhite.end());
	}

	std::set<PieceMove> result;
	for (const Square& dst : possibleSquares)
	{
		result.insert(PieceMove::regularMove(
					rook,
					square,
					dst));
	}

	return result;
}
