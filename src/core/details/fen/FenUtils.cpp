#include "FenUtils.h"
#include <simplechess/Exceptions.h>

#include "../../../core/Builders.h"
#include "FenParser.h"

#include <simplechess/GameStage.h>

#include <stdexcept>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	/**
	 * Generates the first four fields of a FEN string (piece placement,
	 * active color, castling availability and en passant target), which are
	 * shared between a full FEN and an n-fold repetition key.
	 */
	std::string generateFenPrefix(
			const Board& board,
			const Color activeColor,
			const uint8_t castlingRights,
			const std::optional<Square>& epTarget);
}

char FenUtils::pieceToString(const Piece& piece)
{
	// One character per piece type, in the order of the PieceType enum;
	// upper case for white, lower case for black.
	static const char letters[] = "PRNBQK";

	const size_t type = static_cast<size_t>(piece.type());

	if (type >= sizeof(letters) - 1)
	{
		throw simplechess::InvalidArgumentException("Cannot convert piece to FEN");
	}

	return (piece.color() == Color::White)
		? letters[type]
		: static_cast<char>(letters[type] - 'A' + 'a');
}

Piece FenUtils::stringToPiece(const char c)
{
	const bool isWhite = (c >= 'A' && c <= 'Z');
	const char upper = isWhite ? c : static_cast<char>(c - 'a' + 'A');
	const Color color = isWhite ? Color::White : Color::Black;

	switch (upper)
	{
		case 'P': return {PieceType::Pawn, color};
		case 'R': return {PieceType::Rook, color};
		case 'N': return {PieceType::Knight, color};
		case 'B': return {PieceType::Bishop, color};
		case 'Q': return {PieceType::Queen, color};
		case 'K': return {PieceType::King, color};
		default: break;
	}

	throw simplechess::InvalidArgumentException(std::string("Character \'")
			+ c
			+ "\' is not a piece-representing character in FEN notation");
}

std::string FenUtils::fenForRepetitions(const std::string& fen)
{
	// The repetition key is the FEN without its last two fields, so rather
	// than splitting the string into all six and joining four back together,
	// find where the fifth field starts and cut there.
	const size_t afterEpTarget = fen.rfind(' ', fen.rfind(' ') - 1);

	if (afterEpTarget == std::string::npos)
	{
		throw simplechess::InvalidArgumentException(fen + " is not a valid FEN string");
	}

	return fen.substr(0, afterEpTarget);
}

std::string FenUtils::repetitionKey(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const std::optional<Square>& epTarget)
{
	// The repetition key is exactly the first four fields of the FEN string:
	// everything which defines a position for the purposes of the n-fold
	// repetition rule (piece placement, side to move, castling rights and en
	// passant possibility), and nothing which does not (the two counters).
	return internal::generateFenPrefix(
			board, activeColor, castlingRights, epTarget);
}

std::string FenUtils::generateFen(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const std::optional<Square>& epTarget,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock)
{
	// A FEN string is an ASCII string composed of six fields, separated from
	// each other by a space. The first four are shared with the repetition
	// key; only the two counters are appended here.
	std::string fen = internal::generateFenPrefix(
			board, activeColor, castlingRights, epTarget);

	// 5. Halfmove clock: The number of halfmoves since the last capture or
	// pawn advance, used for the fifty-move rule.
	fen += ' ';
	fen += std::to_string(halfmoveClock);

	// 6. Fullmove number: The number of the full move. It starts at 1, and is
	// incremented after Black's move.
	fen += ' ';
	fen += std::to_string(fullmoveClock);

	return fen;
}

std::string internal::generateFenPrefix(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const std::optional<Square>& epTarget)
{
	// A FEN never exceeds ~90 characters, so reserving up front means the
	// string is never reallocated while it is assembled. This runs for every
	// position the library builds.
	std::string fen;
	fen.reserve(90);

	// From Wikipedia:
	// 1. Piece placement (from White's perspective). Each rank is described,
	// starting with rank 8 and ending with rank 1; within each rank, the
	// contents of each square are described from file "a" through file "h".
	// Following the Standard Algebraic Notation (SAN), each piece is
	// identified by a single letter taken from the standard English names
	// (pawn = "P", knight = "N", bishop = "B", rook = "R", queen = "Q" and
	// king = "K"). White pieces are designated using upper-case letters
	// ("PNBRQK") while black pieces use lowercase ("pnbrqk"). Empty squares
	// are noted using digits 1 through 8 (the number of empty squares), and
	// "/" separates ranks.

	uint8_t emptySquaresRun = 0;

	for (uint8_t rank = 8; rank > 0; --rank)
	{
		for (char file = 'a'; file <= 'h'; ++file)
		{
			const Square square = Square::fromRankAndFile(rank, file);

			const std::optional<Piece> piece = board.pieceAt(square);

			if (!piece)
			{
				emptySquaresRun++;
				continue;
			}

			// We have encountered a piece, list number of empty squares if any
			if (emptySquaresRun != 0)
			{
				fen += static_cast<char>('0' + emptySquaresRun);
				emptySquaresRun = 0;
			}

			fen += FenUtils::pieceToString(*piece);
		}

		// We have finished a rank, list number of empty squares if any
		if (emptySquaresRun != 0)
		{
			fen += static_cast<char>('0' + emptySquaresRun);
			emptySquaresRun = 0;
		}

		if (rank != 1)
		{
			fen += '/';
		}
	}

	// 2. Active color. "w" means White moves next, "b" means Black moves next.
	fen += ' ';
	fen += (activeColor == Color::White) ? 'w' : 'b';

	// 3. Castling availability. If neither side can castle, this is "-".
	// Otherwise, this has one or more letters: "K" (White can castle
	// kingside), "Q" (White can castle queenside), "k" (Black can castle
	// kingside), and/or "q" (Black can castle queenside). A move that
	// temporarily prevents castling does not negate this notation.
	fen += ' ';
	if (castlingRights == 0)
	{
		fen += '-';
	}
	else
	{
		if ((castlingRights & CastlingRight::WhiteKingSide) != 0)
		{
			fen += 'K';
		}

		if ((castlingRights & CastlingRight::WhiteQueenSide) != 0)
		{
			fen += 'Q';
		}

		if ((castlingRights & CastlingRight::BlackKingSide) != 0)
		{
			fen += 'k';
		}

		if ((castlingRights & CastlingRight::BlackQueenSide) != 0)
		{
			fen += 'q';
		}
	}
	// 4. En passant target square in algebraic notation. If there's no en
	// passant target square, this is "-". If a pawn has just made a two-square
	// move, this is the position "behind" the pawn. This is recorded
	// regardless of whether there is a pawn in position to make an en passant
	// capture.
	fen += ' ';
	if (epTarget)
	{
		fen += epTarget->toString();
	}
	else
	{
		fen += '-';
	}

	return fen;
}

GameStage FenUtils::fromFenString(const std::string& fen)
{
	const FenParser parsedFen = FenParser::parse(fen);

	return GameStageBuilder::build(
		parsedFen.board(),
		parsedFen.activeColor(),
		parsedFen.castlingRights(),
		parsedFen.halfMovesSinceLastCaptureOrPawnAdvance(),
		parsedFen.fullMoveCounter(),
		parsedFen.enPassantTarget());
}
