#include "KingMove.h"

#include <cpp/simplechess/GameStage.h>
#include "../BoardAnalyzer.h"

using namespace simplechess;
using namespace simplechess::details;

namespace
{
	/**
	 * Whether the king may castle towards \p rookFile.
	 *
	 * Every square between the king and the rook must be vacant, and the two
	 * squares the king travels through must additionally be free of attack.
	 * (Whether the king is currently in check is checked by the caller, once,
	 * for both sides.)
	 */
	bool canCastle(
			const Board& board,
			const Color color,
			const Square& kingSquare,
			const char rookFile)
	{
		const uint8_t rank = kingSquare.rank();
		const bool kingSide = (rookFile == 'h');

		const char firstTravelled = kingSide ? 'f' : 'd';
		const char secondTravelled = kingSide ? 'g' : 'c';

		for (const char file : {firstTravelled, secondTravelled})
		{
			const Square square = Square::fromRankAndFile(rank, file);

			if (!BoardAnalyzer::isEmpty(board, square)
					|| BoardAnalyzer::isSquareThreatenedBy(
						board,
						square,
						oppositeColor(color)))
			{
				return false;
			}
		}

		// Queenside castling also passes over the knight's square, which must
		// be vacant but, unlike the squares the king travels through, may be
		// under attack.
		if (!kingSide
				&& !BoardAnalyzer::isEmpty(
					board,
					Square::fromRankAndFile(rank, 'b')))
		{
			return false;
		}

		return true;
	}
}

void simplechess::details::appendKingMovesExceptCastling(
		std::vector<PieceMove>& moves,
		const Board& board,
		const Color color,
		const Square& square)
{
	const Piece king = {PieceType::King, color};

	for (const int8_t rankStep : {-1, 0, 1})
	{
		for (const int8_t fileStep : {-1, 0, 1})
		{
			if (rankStep == 0 && fileStep == 0)
			{
				continue;
			}

			const int rank = square.rank() + rankStep;
			const int file = square.file() + fileStep;

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
				moves.push_back(PieceMove::regularMove(king, square, dst));
			}
		}
	}
}

void simplechess::details::appendKingMovesUnfiltered(
		std::vector<PieceMove>& moves,
		const Board& board,
		const uint8_t castlingRights,
		const Color color,
		const Square& square)
{
	const Piece king = {PieceType::King, color};

	appendKingMovesExceptCastling(moves, board, color, square);

	const uint8_t kingSideRight = (color == Color::White)
		? CastlingRight::WhiteKingSide
		: CastlingRight::BlackKingSide;

	const uint8_t queenSideRight = (color == Color::White)
		? CastlingRight::WhiteQueenSide
		: CastlingRight::BlackQueenSide;

	if ((castlingRights & (kingSideRight | queenSideRight)) == 0)
	{
		// Nothing to consider, and in particular no need to work out whether
		// the king is in check.
		return;
	}

	if (BoardAnalyzer::isInCheck(board, color))
	{
		// Castling is not available in check
		return;
	}

	if (castlingRights & kingSideRight)
	{
		if (canCastle(board, color, square, 'h'))
		{
			moves.push_back(PieceMove::regularMove(
						king,
						square,
						Square::fromRankAndFile(square.rank(), 'g')));
		}
	}

	if (castlingRights & queenSideRight)
	{
		if (canCastle(board, color, square, 'a'))
		{
			moves.push_back(PieceMove::regularMove(
						king,
						square,
						Square::fromRankAndFile(square.rank(), 'c')));
		}
	}
}
