#include "FenParser.h"
#include "FenUtils.h"

#include <boost/algorithm/string.hpp>

#include <cctype>
#include <cstdint>
#include <vector>

using namespace simplechess;
using namespace simplechess::details;

namespace internal
{
	Board parsePiecePlacement(
			const std::string& piecePlacementFen)
	{
		std::vector<std::string> rows;
		boost::split(rows, piecePlacementFen, [](char c) { return c == '/';});

		if (rows.size() != 8)
		{
			throw std::invalid_argument(piecePlacementFen
					+ " is not a valid \"piece placement\" "
					+ "field in a FEN string");
		}

		std::map<Square, Piece> pieceLocations;

		// In FEN, numbers indicate a run of consecutive empty squares in the
		// row. Numbers must be in [1,8], and cannot be consecutive ('12' is
		// not allowed, as 12 is too large and if it is supposed to represent
		// "1 empty square followed by 2 empty squares" it should simply be
		// '3').
		bool isLastCharNumber = false;

		for (int row = 8; row >= 1; --row)
		{
			isLastCharNumber = false;
			char col = 'a';
			const std::string str = rows[8 - row];

			for (const auto c : str)
			{
				if (std::isdigit(c))
				{
					if (isLastCharNumber)
					{
						// Two digits in a row, error
						throw std::invalid_argument(
								piecePlacementFen
								+ " is not a valid \"piece placement\" "
								+ "field in a FEN string");
					}

					isLastCharNumber = true;

					const uint8_t offset = c - '0';

					if (offset <= 0 || offset > 8)
					{
						throw std::invalid_argument(
								piecePlacementFen
								+ " is not a valid \"piece placement\" "
								+ "field in a FEN string");
					}

					col += offset;

					if (col > ('h' + 1))
					{
						throw std::invalid_argument(
								piecePlacementFen
								+ " is not a valid \"piece placement\" "
								+ "field in a FEN string");
					}

					continue;
				}

				isLastCharNumber = false;

				if (col < 'a' || col > 'h')
				{
					throw std::invalid_argument(
								piecePlacementFen
								+ " is not a valid \"piece placement\" "
								+ "field in a FEN string");
				}
				pieceLocations.insert({
						Square::instantiateWithRankAndFile(row, col),
						FenUtils::stringToPiece(c)});
			}
		}

		return Board(pieceLocations);
	}

	Color parseColor(const std::string& str)
	{
		if (str == "w")
		{
			return COLOR_WHITE;
		}
		else if (str == "b")
		{
			return COLOR_BLACK;
		}

		throw std::invalid_argument(
				str + " is not a valid \"active color\" field in a FEN string");
	}

	uint8_t parseCastlingRights(const std::string& str)
	{
		if (str == "-")
		{
			return 0;
		}

		if (str.size() == 0 || str.size() > 4)
		{
			throw std::invalid_argument(
					str
					+ " is not a valid \"castling availability\" "
					+ "field in a FEN string");
		}

		uint8_t mask = 0;

		for (const auto c : str)
		{
			uint8_t oldMask = mask;
			if (c == 'K')
			{
				mask |= CASTLING_RIGHT_WHITE_KINGSIDE;
			}
			else if (c == 'Q')
			{
				mask |= CASTLING_RIGHT_WHITE_QUEENSIDE;
			}
			else if (c == 'k')
			{
				mask |= CASTLING_RIGHT_BLACK_KINGSIDE;
			}
			else if (c == 'q')
			{
				mask |= CASTLING_RIGHT_BLACK_QUEENSIDE;
			}

			if (oldMask == mask)
			{
				// This was an invalid or a repeated character
				throw std::invalid_argument(
						str
						+ " is not a valid \"castling availability\" "
						+ "field in a FEN string");
			}
		}

		return mask;
	}

	boost::optional<Square> parseEnPassantTarget(const std::string& str)
	{
		if (str == "-")
		{
			return {};
		}

		return Square::instantiateFromString(str);
	}

	uint16_t parseMoveClock(const std::string& str)
	{
		const int number = std::stoi(str);

		if (number < 0 || number > UINT16_MAX)
		{
			throw std::invalid_argument(
					str + " is not a valid \"move clock\" field in a FEN string");
		}

		return static_cast<uint16_t>(number);
	}
}

FenParser::FenParser(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const boost::optional<Square>& enPassantTarget,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock)
	: mBoard(board),
	  mActiveColor(activeColor),
	  mCastlingRights(castlingRights),
	  mEpTarget(enPassantTarget),
	  mHalfmoveClock(halfmoveClock),
	  mFullmoveClock(fullmoveClock)
{
}

FenParser FenParser::parse(const std::string& fen)
{
	std::vector<std::string> tokens;
	boost::split(tokens, fen, [](char c) { return c == ' ';});

	if (tokens.size() != 6)
	{
		throw std::invalid_argument(fen + " is not a valid FEN string");
	}

	const Board board = internal::parsePiecePlacement(tokens[0]);
	const Color activeColor = internal::parseColor(tokens[1]);
	const uint8_t castlingRights = internal::parseCastlingRights(tokens[2]);
	const boost::optional<Square> epTarget = internal::parseEnPassantTarget(tokens[3]);
	const uint16_t halfmoveClock = internal::parseMoveClock(tokens[4]);
	const uint16_t fullmoveClock = internal::parseMoveClock(tokens[5]);

	if (epTarget
			&& ((epTarget->rank() == 3
					&& board.pieceAt(*epTarget) != boost::optional<Piece>({TYPE_PAWN, COLOR_WHITE}))
				|| (epTarget->rank() == 6
					&& board.pieceAt(*epTarget) != boost::optional<Piece>({TYPE_PAWN, COLOR_BLACK}))))
	{
		throw std::invalid_argument(
				"Found inconsistency between piece placement and "
				"\"en passant target\" square in FEN string: "
				+ fen);
	}

	if (halfmoveClock == 0 && epTarget)
	{
		throw std::invalid_argument(
				"Found inconsistency between half move clock and en pasant move "
				"in FEN string: "
				+ fen);
	}

	if (fullmoveClock == 0 && activeColor == COLOR_WHITE && epTarget)
	{
		throw std::invalid_argument(
				"Found inconsistency between full move clock and en pasant move "
				"in FEN string: "
				+ fen);
	}

	return {
		board,
		activeColor,
		castlingRights,
		epTarget,
		halfmoveClock,
		fullmoveClock };
}

const Board& FenParser::board() const
{
	return mBoard;
}

const boost::optional<Square>& FenParser::enPassantTarget() const
{
	return mEpTarget;
}

Color FenParser::activeColor() const
{
	return mActiveColor;
}

uint8_t FenParser::castlingRights() const
{
	return mCastlingRights;
}

uint16_t FenParser::halfMovesSinceLastCaptureOrPawnAdvance() const
{
	return mHalfmoveClock;
}

uint16_t FenParser::fullMoveCounter() const
{
	return mFullmoveClock;
}
