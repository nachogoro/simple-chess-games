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

// TODO improve the "validXXXMoves" scheme, as they are not really valid until
// we filter out checks

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

std::set<PieceMove> MoveValidator::availableMovesForPieceUnfiltered(
		const Board& board,
		const boost::optional<Square>& enPassantTarget,
		const Square& square)
{
	const Color color = board.pieceAt(square)->color();

	switch (board.pieceAt(square)->type())
	{
		case TYPE_PAWN:
			return validPawnMoves(board, enPassantTarget, color, square);
		case TYPE_ROOK:
			return validRookMoves(board, color, square);
		case TYPE_KNIGHT:
			return validKnightMoves(board, color, square);
		case TYPE_BISHOP:
			return validBishopMoves(board, color, square);
		case TYPE_QUEEN:
			return validQueenMoves(board, color, square);
		case TYPE_KING:
			return kingMovesExceptCastling(board, color, square);
	}

	throw std::invalid_argument(
			std::string("Unknown piece type ")
			+ std::to_string(board.pieceAt(square)->type()));
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
			unfiltered = validPawnMoves(board, enPassantTarget, color, square);
			break;
		case TYPE_ROOK:
			unfiltered = validRookMoves(board, color, square);
			break;
		case TYPE_KNIGHT:
			unfiltered = validKnightMoves(board, color, square);
			break;
		case TYPE_BISHOP:
			unfiltered = validBishopMoves(board, color, square);
			break;
		case TYPE_QUEEN:
			unfiltered = validQueenMoves(board, color, square);
			break;
		case TYPE_KING:
			unfiltered = validKingMoves(board, castlingRights, color, square);
			break;
	}

	// Filter out moves which would expose the own king
	std::set<PieceMove> result;

	for (const auto& move : unfiltered)
	{
		const Board afterMove = board.makeMove(move);

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

std::set<PieceMove> MoveValidator::allAvailableMovesUnfiltered(
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
				= availableMovesForPieceUnfiltered(
						board,
						enPassantTarget,
						squarePiece.first);
			result.insert(pieceMoves.begin(), pieceMoves.end());
		}
	}

	return result;
}
