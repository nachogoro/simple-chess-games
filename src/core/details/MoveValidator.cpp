#include "MoveValidator.h"
#include <simplechess/Exceptions.h>

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

namespace
{
	/**
	 * Appends every move the piece on \p square could make if the safety of
	 * its own king were not a consideration.
	 */
	void appendPseudoLegalMoves(
			std::vector<PieceMove>& moves,
			const Board& board,
			const std::optional<Square>& enPassantTarget,
			const uint8_t castlingRights,
			const Square& square,
			const Piece& piece)
	{
		using namespace simplechess::details;

		switch (piece.type())
		{
			case PieceType::Pawn:
				appendPawnMovesUnfiltered(
						moves, board, enPassantTarget, piece.color(), square);
				return;
			case PieceType::Rook:
				appendRookMovesUnfiltered(moves, board, piece.color(), square);
				return;
			case PieceType::Knight:
				appendKnightMovesUnfiltered(moves, board, piece.color(), square);
				return;
			case PieceType::Bishop:
				appendBishopMovesUnfiltered(moves, board, piece.color(), square);
				return;
			case PieceType::Queen:
				appendQueenMovesUnfiltered(moves, board, piece.color(), square);
				return;
			case PieceType::King:
				appendKingMovesUnfiltered(
						moves, board, castlingRights, piece.color(), square);
				return;
		}

		throw simplechess::InvalidArgumentException(
				std::string("Unknown piece type ")
				+ std::to_string(static_cast<int>(piece.type())));
	}

	/**
	 * Drops from \p moves (starting at \p from) every move which would leave
	 * the mover's own king in check.
	 */
	void discardMovesExposingOwnKing(
			std::vector<PieceMove>& moves,
			const size_t from,
			const Board& board,
			const Color color)
	{
		size_t kept = from;

		for (size_t i = from; i < moves.size(); ++i)
		{
			const Board afterMove
				= BoardAnalyzer::makeMoveOnBoard(board, moves[i]);

			if (!BoardAnalyzer::isInCheck(afterMove, color))
			{
				moves[kept++] = moves[i];
			}
		}

		// erase rather than resize: shrinking with resize would still require
		// PieceMove to be default-constructible.
		moves.erase(moves.begin() + static_cast<long>(kept), moves.end());
	}
}

std::vector<PieceMove> MoveValidator::availableMovesForPiece(
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Square& square)
{
	const Piece piece = *board.pieceAt(square);

	std::vector<PieceMove> moves;
	appendPseudoLegalMoves(
			moves, board, enPassantTarget, castlingRights, square, piece);

	discardMovesExposingOwnKing(moves, 0, board, piece.color());

	return moves;
}

std::vector<PieceMove> MoveValidator::allAvailableMoves(
		const Board& board,
		const std::optional<Square>& enPassantTarget,
		const uint8_t castlingRights,
		const Color activeColor)
{
	std::vector<PieceMove> result;

	// Comfortably more than the number of moves available in any ordinary
	// position, so the vector is not reallocated while it is filled.
	result.reserve(64);

	// Scanning the packed array rather than Board::occupiedSquares() keeps
	// this off the map, which would otherwise have to be materialised for
	// every board examined while generating moves.
	const std::array<uint8_t, 64>& squares = BoardAccess::squares(board);

	for (uint8_t index = 0; index < 64; ++index)
	{
		const uint8_t code = squares[index];

		if (!BoardAccess::isColor(code, activeColor))
		{
			continue;
		}

		// Each piece contributes moves from its own square, so no two pieces
		// can produce the same move and no de-duplication is needed.
		const size_t before = result.size();

		appendPseudoLegalMoves(
				result,
				board,
				enPassantTarget,
				castlingRights,
				BoardAccess::squareOf(index),
				BoardAccess::decodePiece(code));

		discardMovesExposingOwnKing(result, before, board, activeColor);
	}

	return result;
}
