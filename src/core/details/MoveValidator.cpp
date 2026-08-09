#include "MoveValidator.h"

#include "BoardAccess.h"
#include "BoardAnalyzer.h"

#include "moves/BishopMove.h"
#include "moves/KingMove.h"
#include "moves/KnightMove.h"
#include "moves/PawnMove.h"
#include "moves/QueenMove.h"
#include "moves/RookMove.h"

#include <stdexcept>

using namespace simplechess;
using namespace simplechess::details;

PositionAnalysis details::analyzePosition(
		const Board& board,
		const Color activeColor,
		const std::optional<Square>& enPassantTarget,
		const uint8_t castlingRights)
{
	const bool inCheck = BoardAnalyzer::isInCheck(board, activeColor);

	std::vector<PieceMove> legalMoves = MoveValidator::allAvailableMoves(
			board,
			enPassantTarget,
			castlingRights,
			activeColor);

	const CheckType checkType = inCheck
		? (legalMoves.empty() ? CheckType::CheckMate : CheckType::Check)
		: CheckType::NoCheck;

	return {checkType, std::move(legalMoves)};
}

uint8_t details::updatedCastlingRights(
		uint8_t castlingRights,
		const PieceMove& move)
{
	if (move.piece().type() == PieceType::King)
	{
		// Once the king moves, castling is no longer allowed
		if (move.piece().color() == Color::White)
		{
			castlingRights &= ~CastlingRight::WhiteKingSide;
			castlingRights &= ~CastlingRight::WhiteQueenSide;
		}
		else
		{
			castlingRights &= ~CastlingRight::BlackKingSide;
			castlingRights &= ~CastlingRight::BlackQueenSide;
		}
	}

	// If the move starts or ends in a rook's original square, castling rights
	// are lost. Comparing rank and file directly avoids parsing a square out
	// of a string on every move.
	const auto touches = [&move](const uint8_t rank, const char file) {
		return (move.src().rank() == rank && move.src().file() == file)
			|| (move.dst().rank() == rank && move.dst().file() == file);
	};

	if (touches(1, 'a'))
	{
		castlingRights &= ~CastlingRight::WhiteQueenSide;
	}

	if (touches(1, 'h'))
	{
		castlingRights &= ~CastlingRight::WhiteKingSide;
	}

	if (touches(8, 'a'))
	{
		castlingRights &= ~CastlingRight::BlackQueenSide;
	}

	if (touches(8, 'h'))
	{
		castlingRights &= ~CastlingRight::BlackKingSide;
	}

	return castlingRights;
}

std::optional<Square> MoveValidator::enPassantTarget(
		const Board& board,
		const PieceMove& pieceMove)
{
	if (pieceMove.piece().type() == PieceType::Pawn
			&& abs(pieceMove.dst().rank() - pieceMove.src().rank()) == 2)
	{
		const Square candidateTarget = Square::fromRankAndFile(
				((pieceMove.piece().color() == Color::White)
				 ? 3
				 : 6),
				pieceMove.dst().file());

		// Only report the en passant target if an enemy pawn can legally
		// capture en passant (i.e. adjacent and not pinned)
		const Color enemyColor = oppositeColor(pieceMove.piece().color());
		const Piece enemyPawn = {PieceType::Pawn, enemyColor};
		const uint8_t dstRank = pieceMove.dst().rank();
		const char dstFile = pieceMove.dst().file();

		for (int fileDelta : {-1, 1})
		{
			const char adjFile = dstFile + fileDelta;
			if (!Square::isInsideBoundaries(dstRank, adjFile))
				continue;

			const Square adjSquare = Square::fromRankAndFile(dstRank, adjFile);
			if (board.pieceAt(adjSquare) != std::optional<Piece>(enemyPawn))
				continue;

			// Check if this pawn has a legal en passant capture
			const std::vector<PieceMove> moves = availableMovesForPiece(
					board, candidateTarget, 0, adjSquare);

			for (const auto& move : moves)
			{
				if (move.dst() == candidateTarget)
					return candidateTarget;
			}
		}
	}

	return {};
}

std::vector<PieceMove> MoveValidator::availableMovesForPiece(
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Square& square)
{
	const Color color = board.pieceAt(square)->color();

	std::vector<PieceMove> unfiltered;

	switch (board.pieceAt(square)->type())
	{
		case PieceType::Pawn:
			appendPawnMovesUnfiltered(
					unfiltered, board, enPassantTarget, color, square);
			break;
		case PieceType::Rook:
			appendRookMovesUnfiltered(unfiltered, board, color, square);
			break;
		case PieceType::Knight:
			appendKnightMovesUnfiltered(unfiltered, board, color, square);
			break;
		case PieceType::Bishop:
			appendBishopMovesUnfiltered(unfiltered, board, color, square);
			break;
		case PieceType::Queen:
			appendQueenMovesUnfiltered(unfiltered, board, color, square);
			break;
		case PieceType::King:
			appendKingMovesUnfiltered(
					unfiltered, board, castlingRights, color, square);
			break;
	}

	// Filter out moves which would expose the own king
	std::vector<PieceMove> result;

	for (const auto& move : unfiltered)
	{
		const Board afterMove = BoardAnalyzer::makeMoveOnBoard(board, move);

		if (!BoardAnalyzer::isInCheck(afterMove, color))
		{
			result.push_back(move);
		}
	}

	return result;
}

std::vector<PieceMove> MoveValidator::allAvailableMoves(
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Color activeColor)
{
	std::vector<PieceMove> result;

	const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

	for (uint8_t index = 0; index < 64; ++index)
	{
		if (BoardAccess::isColor(squares[index], activeColor))
		{
			const std::vector<PieceMove> pieceMoves = availableMovesForPiece(
					board,
					enPassantTarget,
					castlingRights,
					BoardAccess::squareOf(index));
			result.insert(result.end(), pieceMoves.begin(), pieceMoves.end());
		}
	}

	return result;
}
