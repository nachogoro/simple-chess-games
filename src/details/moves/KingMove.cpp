#include "KingMove.h"

#include <GameStage.h>
#include <details/BoardAnalyzer.h>

using namespace simplechess;
using namespace simplechess::details;

std::set<PieceMove> simplechess::details::kingMovesExceptCastling(
		const Board& board,
		const Color color,
		const Square& square)
{
	const std::set<int8_t> step = {-1, 0, 1};

	const Piece king = {TYPE_KING, color};

	std::set<PieceMove> result;

	for (const int8_t rankStep : step)
	{
		for (const int8_t fileStep : step)
		{
			if (Square::isInsideBoundaries(
						square.rank() + rankStep,
						square.file() + fileStep))
			{
				const Square dst = Square::fromRankAndFile(
						square.rank() + rankStep,
						square.file() + fileStep);

				if (BoardAnalyzer::isOccupiableBy(board, dst, color))
				{
					// The square currently occupied by the king does not make
					// it past this if's guard
					result.insert(PieceMove::regularMove(
								king,
								square,
								dst));
				}
			}
		}
	}

	return result;
}

std::set<PieceMove> simplechess::details::validKingMoves(
		const Board& board,
		const uint8_t castlingRights,
		const Color color,
		const Square& square)
{
	const Piece king = {TYPE_KING, color};

	std::set<PieceMove> result = kingMovesExceptCastling(board, color, square);

	if (BoardAnalyzer::isInCheck(board, color))
	{
		// Castling is not available in check
		return result;
	}

	if ((color == COLOR_WHITE
				&& (castlingRights & CASTLING_RIGHT_WHITE_KINGSIDE))
			|| (color == COLOR_BLACK
				&& (castlingRights & CASTLING_RIGHT_BLACK_KINGSIDE)))
	{
		// Only available if the passing squares are empty and not under attack
		const std::set<Square> mustBeFreeSquares = {
			Square::fromRankAndFile(square.rank(), 'f'),
			Square::fromRankAndFile(square.rank(), 'g')
		};

		bool allClear = true;

		for (const Square& sq : mustBeFreeSquares)
		{
			if (!BoardAnalyzer::isEmpty(board, sq)
					|| BoardAnalyzer::isSquareThreatenedBy(
						board,
						sq,
						oppositeColor(color)))
			{
				allClear = false;
			}
		}

		if (allClear)
		{
			result.insert(PieceMove::regularMove(
						king,
						square,
						Square::fromRankAndFile(
							square.rank(),
							'g')));
		}
	}

	if ((color == COLOR_WHITE
				&& (castlingRights & CASTLING_RIGHT_WHITE_QUEENSIDE))
			|| (color == COLOR_BLACK
				&& (castlingRights & CASTLING_RIGHT_BLACK_QUEENSIDE)))
	{
		// Only available if the passing squares are empty and not under attack
		const std::set<Square> mustBeFreeSquares = {
			Square::fromRankAndFile(square.rank(), 'd'),
			Square::fromRankAndFile(square.rank(), 'c')
		};

		bool allClear = true;

		for (const Square& sq : mustBeFreeSquares)
		{
			if (!BoardAnalyzer::isEmpty(board, sq)
					|| BoardAnalyzer::isSquareThreatenedBy(
						board,
						sq,
						oppositeColor(color)))
			{
				allClear = false;
			}
		}

		if (allClear)
		{
			result.insert(PieceMove::regularMove(
						king,
						square,
						Square::fromRankAndFile(
							square.rank(),
							'c')));
		}
	}

	return result;
}
