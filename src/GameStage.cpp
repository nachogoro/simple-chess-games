#include "GameStage.h"

#include <cctype>
#include <sstream>

using namespace simplechess;

namespace internal
{
	char fenDescription(const Piece piece) {
		char result;

		switch (piece.type())
		{
			case TYPE_PAWN:
				result = 'p';
				break;
			case TYPE_ROOK:
				result = 'r';
				break;
			case TYPE_KNIGHT:
				result = 'n';
				break;
			case TYPE_BISHOP:
				result = 'b';
				break;
			case TYPE_QUEEN:
				result = 'q';
				break;
			case TYPE_KING:
				result = 'k';
				break;
		}

		return piece.color() == COLOR_WHITE
			? std::toupper(result)
			: result;
	}
}

GameStage::GameStage(
		const Board& board,
		const Color toPlay,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const boost::optional<PlayedMove>& move)
	: mBoard(board),
	  mActiveColor(toPlay),
	  mCastlingRights(castlingRights),
	  mHalfmoveClock(halfmoveClock),
	  mFullmoveClock(fullmoveClock),
	  mMove(move),
	  mFen(generateFen())
{
}

const Board& GameStage::board() const
{
	return mBoard;
}

const boost::optional<PlayedMove>& GameStage::move() const
{
	return mMove;
}

Color GameStage::activeColor() const
{
	return mActiveColor;
}

uint8_t GameStage::castlingRights() const
{
	return mCastlingRights;
}

uint16_t GameStage::halfMovesSinceLastCaptureOrPawnAdvance() const
{
	return mHalfmoveClock;
}

const std::string& GameStage::fen() const
{
	return mFen;
}

std::string GameStage::generateFen() const
{
	// A FEN string is an ASCII string composed of six fields, separated from
	// each other by a space.

	std::stringstream ss;

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

	for (uint8_t rank = 8; rank > 0; ++rank)
	{
		for (char file = 'a'; file <= 'h'; ++file)
		{
			const Square square
				= Square::instantiateWithRankAndFile(rank, file);

			const boost::optional<Piece> piece = board().pieceAt(square);

			if (!piece)
			{
				emptySquaresRun++;
				continue;
			}

			// We have encountered a piece, list number of empty squares if any
			if (emptySquaresRun != 0)
			{
				ss << emptySquaresRun;
				emptySquaresRun = 0;
			}

			ss << ::internal::fenDescription(*piece);
		}

		// We have finished a rank, list number of empty squares if any
		if (emptySquaresRun != 0)
		{
			ss << emptySquaresRun;
			emptySquaresRun = 0;
		}

		ss << "/";
	}
	// 2. Active color. "w" means White moves next, "b" means Black moves next.
	// 3. Castling availability. If neither side can castle, this is "-".
	// Otherwise, this has one or more letters: "K" (White can castle
	// kingside), "Q" (White can castle queenside), "k" (Black can castle
	// kingside), and/or "q" (Black can castle queenside). A move that
	// temporarily prevents castling does not negate this notation.
	// 4. En passant target square in algebraic notation. If there's no en
	// passant target square, this is "-". If a pawn has just made a two-square
	// move, this is the position "behind" the pawn. This is recorded
	// regardless of whether there is a pawn in position to make an en passant
	// capture.
	// 5. Halfmove clock: The number of halfmoves since the last capture or
	// pawn advance, used for the fifty-move rule.
	// 6. Fullmove number: The number of the full move. It starts at 1, and is
	// incremented after Black's move.

}
