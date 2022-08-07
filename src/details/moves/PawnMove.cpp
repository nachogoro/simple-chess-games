#include "PawnMove.h"

#include <details/BoardAnalyzer.h>

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::pawnMovesUnfiltered(
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const Color color,
		const Square& square)
{
	std::set<Square> finalSquares;

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

	const Square oneAhead = Square::fromRankAndFile(
			square.rank() + step,
			square.file());

	if (BoardAnalyzer::isEmpty(board, oneAhead))
	{
		finalSquares.insert(oneAhead);
	}

	if (pawn.color() == Color::White && square.rank() == 2
			|| pawn.color() == Color::Black && square.rank() == 7)
	{
		// The pawn has never moved, might be able to move twice ahead
		const Square twoAhead = Square::fromRankAndFile(
				square.rank() + 2*step,
				square.file());

		if (BoardAnalyzer::isEmpty(board, oneAhead)
				&& BoardAnalyzer::isEmpty(board, twoAhead))
		{
			finalSquares.insert(twoAhead);
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
			finalSquares.insert(aheadQueenSide);
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
			finalSquares.insert(aheadKingSide);
		}
	}

	static const std::set<PieceType> sPromotionableTypes = {
		PieceType::Rook,
		PieceType::Knight,
		PieceType::Bishop,
		PieceType::Queen};

	std::set<PieceMove> result;
	for (const Square& dst : finalSquares)
	{
		if (dst.rank() == 1 || dst.rank() == 8)
		{
			// Pawn promotion
			for (const PieceType& promotionType : sPromotionableTypes)
			{
				result.insert(PieceMove::pawnPromotion(
							pawn,
							square,
							dst,
							promotionType));
			}
		}
		else
		{
			result.insert(PieceMove::regularMove(pawn, square, dst));
		}
	}

	return result;
}
