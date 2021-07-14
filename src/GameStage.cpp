#include "GameStage.h"

#include "details/BoardAnalyzer.h"

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
		const std::string& fen,
		const boost::optional<PlayedMove>& move)
	: mBoard(board),
	  mActiveColor(toPlay),
	  mCastlingRights(castlingRights),
	  mHalfmoveClock(halfmoveClock),
	  mFullmoveClock(fullmoveClock),
	  mFen(generateFen()),
	  mMove(move)
{
	// Validate that the state is correct:
	// 1. One and only one King per side
	// 2. The color to move cannot be checking the opposite King
	// 3. If one type of castling is available, both the king and the rook
	// should be in place
	// 4. Move clocks are consistent
	uint8_t whiteKings = 0;
	uint8_t blackKings = 0;

	for (const auto& entry : board.occupiedSquares())
	{
		if (entry.second.type() == TYPE_KING)
		{
			if (entry.second.color() == COLOR_WHITE)
			{
				whiteKings++;
			}
			else
			{
				blackKings++;
			}
		}
	}

	if (whiteKings != 1 || blackKings != 1)
	{
		throw std::invalid_argument("Invalid number of kings on board");
	}

	if (details::BoardAnalyzer::isInCheck(board, oppositeColor(toPlay)))
	{
		throw std::invalid_argument("Color to move is already checking");
	}

	const Piece whiteKing = {TYPE_KING, COLOR_WHITE};
	const Piece blackKing = {TYPE_KING, COLOR_BLACK};
	const Piece whiteRook = {TYPE_ROOK, COLOR_WHITE};
	const Piece blackRook = {TYPE_ROOK, COLOR_BLACK};

	if ((castlingRights & CASTLING_RIGHT_WHITE_KINGSIDE)
			&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
				|| *board.pieceAt(Square::fromString("h1")) != whiteRook))
	{
		throw std::invalid_argument(
				"Kingside castling right for white is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_WHITE_QUEENSIDE)
			&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
				|| *board.pieceAt(Square::fromString("a1")) != whiteRook))
	{
		throw std::invalid_argument(
				"Queenside castling right for white is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_BLACK_KINGSIDE)
			&& (*board.pieceAt(Square::fromString("e8")) != blackKing
				|| *board.pieceAt(Square::fromString("h8")) != blackRook))
	{
		throw std::invalid_argument(
				"Kingside castling right for black is inconsistent with board state");
	}

	if ((castlingRights & CASTLING_RIGHT_BLACK_QUEENSIDE)
			&& (*board.pieceAt(Square::fromString("e8")) != blackKing
				|| *board.pieceAt(Square::fromString("a8")) != blackRook))
	{
		throw std::invalid_argument(
				"Queenside castling right for black is inconsistent with board state");
	}
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

uint16_t GameStage::fullMoveCounter() const
{
	return mFullmoveClock;
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

	for (uint8_t rank = 8; rank > 0; --rank)
	{
		for (char file = 'a'; file <= 'h'; ++file)
		{
			const Square square
				= Square::fromRankAndFile(rank, file);

			const boost::optional<Piece> piece = board().pieceAt(square);

			if (!piece)
			{
				emptySquaresRun++;
				continue;
			}

			// We have encountered a piece, list number of empty squares if any
			if (emptySquaresRun != 0)
			{
				ss << static_cast<int>(emptySquaresRun);
				emptySquaresRun = 0;
			}

			ss << ::internal::fenDescription(*piece);
		}

		// We have finished a rank, list number of empty squares if any
		if (emptySquaresRun != 0)
		{
			ss << static_cast<int>(emptySquaresRun);
			emptySquaresRun = 0;
		}

		if (rank != 1)
		{
			ss << "/";
		}
	}

	// 2. Active color. "w" means White moves next, "b" means Black moves next.
	ss << " ";
	ss << ((activeColor() == COLOR_WHITE)
		? "w"
		: "b");

	// 3. Castling availability. If neither side can castle, this is "-".
	// Otherwise, this has one or more letters: "K" (White can castle
	// kingside), "Q" (White can castle queenside), "k" (Black can castle
	// kingside), and/or "q" (Black can castle queenside). A move that
	// temporarily prevents castling does not negate this notation.
	ss << " ";
	if (castlingRights() == 0)
	{
		ss << "-";
	}
	else
	{
		if ((castlingRights() & CASTLING_RIGHT_WHITE_KINGSIDE) != 0)
		{
			ss << "K";
		}

		if ((castlingRights() & CASTLING_RIGHT_WHITE_QUEENSIDE) != 0)
		{
			ss << "Q";
		}

		if ((castlingRights() & CASTLING_RIGHT_BLACK_KINGSIDE) != 0)
		{
			ss << "k";
		}

		if ((castlingRights() & CASTLING_RIGHT_BLACK_QUEENSIDE) != 0)
		{
			ss << "q";
		}
	}
	// 4. En passant target square in algebraic notation. If there's no en
	// passant target square, this is "-". If a pawn has just made a two-square
	// move, this is the position "behind" the pawn. This is recorded
	// regardless of whether there is a pawn in position to make an en passant
	// capture.
	ss << " ";
	if (move()
			&& move()->pieceMove().piece().type() == TYPE_PAWN
			&& abs(move()->pieceMove().dst().rank()
					- move()->pieceMove().src().rank()) == 2)
	{
		ss << move()->pieceMove().dst().file()
			<< ((move()->pieceMove().piece().color() == COLOR_WHITE)
				? 3
				: 6);
	}
	else
	{
		ss << "-";
	}

	// 5. Halfmove clock: The number of halfmoves since the last capture or
	// pawn advance, used for the fifty-move rule.
	ss << " " << halfMovesSinceLastCaptureOrPawnAdvance();

	// 6. Fullmove number: The number of the full move. It starts at 1, and is
	// incremented after Black's move.
	ss << " " << fullMoveCounter();

	return ss.str();
}

GameStage GameStage::makeMove(const PieceMove& move, bool offerDraw) const
{
	const PlayedMove playedMove = PlayedMove::instantiate(
				board(),
				move,
				offerDraw);

	uint8_t updatedCastlingRights = castlingRights();

	if (move.piece().type() == TYPE_KING)
	{
		// Once the king moves, castling is no longer allowed
		if (move.piece().color() == COLOR_WHITE)
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_KINGSIDE;
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_QUEENSIDE;
		}
		else
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_KINGSIDE;
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_QUEENSIDE;
		}
	}

	if (move.piece().type() == TYPE_ROOK)
	{
		// If a rook moves or is captured, clear the castling rights of that
		// side. It might already be cleared, but that's not an issue
		if (move.src() == Square::fromString("a1")
				|| move.dst() == Square::fromString("a1"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_QUEENSIDE;
		}

		if (move.src() == Square::fromString("h1")
				|| move.dst() == Square::fromString("h1"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_WHITE_KINGSIDE;
		}

		if (move.src() == Square::fromString("a8")
				|| move.dst() == Square::fromString("a8"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_QUEENSIDE;
		}

		if (move.src() == Square::fromString("h8")
				|| move.dst() == Square::fromString("h8"))
		{
			updatedCastlingRights &= ~CASTLING_RIGHT_BLACK_KINGSIDE;
		}
	}

	return GameStage(
			details::BoardAnalyzer::makeMoveOnBoard(board(), move),
			oppositeColor(activeColor()),
			updatedCastlingRights,
			(move.piece().type() == TYPE_PAWN || playedMove.capturedPiece())
				? 0
				: halfMovesSinceLastCaptureOrPawnAdvance() + 1,
			fullMoveCounter() + (activeColor() == COLOR_BLACK ? 1 : 0),
			playedMove);
}
