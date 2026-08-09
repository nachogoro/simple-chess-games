#include "PawnMove.h"

#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

void simplechess::details::appendPawnMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const Color color,
		const Square& square)
{
	// A pawn can move:
	//  1. One square ahead if the landing square is empty
	//  2. Two squares ahead if both squares in front of it are empty and it's
	//     its first move
	//  3. Diagonally one square ahead if the square is occupied by a piece of
	//     the opposite color
	//  4. Diagonally one square ahead, behind a pawn of the opposite color
	//     which has just moved two squares ahead (en passant capture)
	//
	// Note that a pawn might promote by reaching the last rank. In that case,
	// all possible promotion moves should be reported.
	const Piece pawn = {PieceType::Pawn, color};
	const int step = pawn.color() == Color::White
		? 1
		: -1;

	// At most four squares are reachable, and they are all distinct.
	Square finalSquares[4] = {square, square, square, square};
	size_t finalSquareCount = 0;

	const Square oneAhead = Square::fromRankAndFile(
			square.rank() + step,
			square.file());

	if (BoardAnalyzer::isEmpty(board, oneAhead))
	{
		finalSquares[finalSquareCount++] = oneAhead;

		if ((pawn.color() == Color::White && square.rank() == 2)
				|| (pawn.color() == Color::Black && square.rank() == 7))
		{
			// The pawn has never moved, might be able to move twice ahead
			const Square twoAhead = Square::fromRankAndFile(
					square.rank() + 2*step,
					square.file());

			if (BoardAnalyzer::isEmpty(board, twoAhead))
			{
				finalSquares[finalSquareCount++] = twoAhead;
			}
		}
	}

	if (square.file() != 'a')
	{
		// Can potentially capture towards the queen side
		const Square aheadQueenSide = Square::fromRankAndFile(
				square.rank() + step,
				square.file() - 1);

		if (BoardAnalyzer::isOccupiedByPieceOfColor(
					board,
					aheadQueenSide,
					oppositeColor(pawn.color()))
				|| (enPassantTarget && enPassantTarget == aheadQueenSide))
		{
			finalSquares[finalSquareCount++] = aheadQueenSide;
		}
	}

	if (square.file() != 'h')
	{
		// Can potentially capture towards the king side
		const Square aheadKingSide = Square::fromRankAndFile(
				square.rank() + step,
				square.file() + 1);

		if (BoardAnalyzer::isOccupiedByPieceOfColor(
					board,
					aheadKingSide,
					oppositeColor(pawn.color()))
				|| (enPassantTarget && enPassantTarget == aheadKingSide))
		{
			finalSquares[finalSquareCount++] = aheadKingSide;
		}
	}

	static const PieceType promotionTypes[4] = {
		PieceType::Rook,
		PieceType::Knight,
		PieceType::Bishop,
		PieceType::Queen};

	for (size_t i = 0; i < finalSquareCount; ++i)
	{
		const Square& dst = finalSquares[i];

		if (dst.rank() == 1 || dst.rank() == 8)
		{
			// Pawn promotion
			for (const PieceType promotionType : promotionTypes)
			{
				moves.push_back(PieceMove::pawnPromotion(
							pawn,
							square,
							dst,
							promotionType));
			}
		}
		else
		{
			moves.push_back(PieceMove::regularMove(pawn, square, dst));
		}
	}
}
