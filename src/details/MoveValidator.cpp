#include "MoveValidator.h"

#include <details/BoardAnalyzer.h>

#include <details/moves/BishopMove.h>
#include <details/moves/KingMove.h>
#include <details/moves/KnightMove.h>
#include <details/moves/PawnMove.h>
#include <details/moves/QueenMove.h>
#include <details/moves/RookMove.h>

using namespace simplechess;
using namespace simplechess::details;

boost::optional<Square> MoveValidator::enPassantTarget(
		const PieceMove& pieceMove)
{
	if (pieceMove.piece().type() == TYPE_PAWN
			&& abs(pieceMove.dst().rank() - pieceMove.src().rank()) == 2)
	{
		return { Square::fromRankAndFile(
				((pieceMove.piece().color() == COLOR_WHITE)
				 ? 3
				 : 6),
				pieceMove.dst().file()) };
	}

	return {};
}

std::set<PieceMove> MoveValidator::potentiallyCapturingMovesForPieceUnfiltered(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		const Square& square)
{
	const Color color = board.pieceAt(square)->color();

	switch (board.pieceAt(square)->type())
	{
		case TYPE_PAWN:
			return pawnMovesUnfiltered(board, enPassantTarget, color, square);
		case TYPE_ROOK:
			return rookMovesUnfiltered(board, color, square);
		case TYPE_KNIGHT:
			return knightMovesUnfiltered(board, color, square);
		case TYPE_BISHOP:
			return bishopMovesUnfiltered(board, color, square);
		case TYPE_QUEEN:
			return queenMovesUnfiltered(board, color, square);
		case TYPE_KING:
			return kingMovesExceptCastling(board, color, square);
	}

	throw std::invalid_argument(
			std::string("Unknown piece type ")
			+ std::to_string(board.pieceAt(square)->type()));
}

std::set<PieceMove> MoveValidator::allPotentiallyCapturingMovesUnfiltered(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		const Color activeColor)
{
	std::set<PieceMove> result;

	for (const auto& squarePiece : board.occupiedSquares())
	{
		if (squarePiece.second.color() == activeColor)
		{
			const std::set<PieceMove> pieceMoves
				= potentiallyCapturingMovesForPieceUnfiltered(
						board,
						enPassantTarget,
						squarePiece.first);
			result.insert(pieceMoves.begin(), pieceMoves.end());
		}
	}

	return result;
}

std::set<PieceMove> MoveValidator::availableMovesForPiece(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Square& square)
{
	const Color color = board.pieceAt(square)->color();

	std::set<PieceMove> unfiltered;

	switch (board.pieceAt(square)->type())
	{
		case TYPE_PAWN:
			unfiltered = pawnMovesUnfiltered(board, enPassantTarget, color, square);
			break;
		case TYPE_ROOK:
			unfiltered = rookMovesUnfiltered(board, color, square);
			break;
		case TYPE_KNIGHT:
			unfiltered = knightMovesUnfiltered(board, color, square);
			break;
		case TYPE_BISHOP:
			unfiltered = bishopMovesUnfiltered(board, color, square);
			break;
		case TYPE_QUEEN:
			unfiltered = queenMovesUnfiltered(board, color, square);
			break;
		case TYPE_KING:
			unfiltered = kingMovesUnfiltered(board, castlingRights, color, square);
			break;
	}

	// Filter out moves which would expose the own king
	std::set<PieceMove> result;

	for (const auto& move : unfiltered)
	{
		const Board afterMove = BoardAnalyzer::makeMoveOnBoard(board, move);

		if (!BoardAnalyzer::isInCheck(afterMove, color))
		{
			result.insert(move);
		}
	}

	return result;
}

std::set<PieceMove> MoveValidator::allAvailableMoves(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Color activeColor)
{
	std::set<PieceMove> result;

	for (const auto& squarePiece : board.occupiedSquares())
	{
		if (squarePiece.second.color() == activeColor)
		{
			const std::set<PieceMove> pieceMoves = availableMovesForPiece(
					board,
					enPassantTarget,
					castlingRights,
					squarePiece.first);
			result.insert(pieceMoves.begin(), pieceMoves.end());
		}
	}

	return result;
}
