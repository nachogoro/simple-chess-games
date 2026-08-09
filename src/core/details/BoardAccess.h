#ifndef BOARD_ACCESS_H_5C6A0D2E_4F1B_47A9_9B7E_0C3E9D5A1F84
#define BOARD_ACCESS_H_5C6A0D2E_4F1B_47A9_9B7E_0C3E9D5A1F84

#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/Piece.h>
#include <cpp/simplechess/Square.h>

#include <array>
#include <cstdint>

namespace simplechess
{
	/**
	 * \brief Internal access to the packed representation behind a \ref Board.
	 *
	 * \ref Board exposes squares one at a time as \c std::optional<Piece>,
	 * which is the right interface for users of the library but too slow for
	 * the inner loops of move generation, where the board is scanned and
	 * copied constantly. This grants the library's own internals direct
	 * access to the underlying bytes without widening the public interface.
	 */
	struct BoardAccess
	{
		/**
		 * \brief The index \p square occupies in the packed array.
		 */
		static uint8_t indexOf(const Square& square)
		{
			return static_cast<uint8_t>(
					(square.rank() - 1) * 8 + (square.file() - 'a'));
		}

		/**
		 * \brief The square at \p index in the packed array.
		 */
		static Square squareOf(const uint8_t index)
		{
			return Square::fromRankAndFile(
					static_cast<uint8_t>(1 + index / 8),
					static_cast<char>('a' + index % 8));
		}

		/**
		 * \brief The index of the square \p fileDelta files and \p rankDelta
		 * ranks away from \p index, or -1 if that would leave the board.
		 *
		 * Both the rank and the file are checked, so this cannot silently wrap
		 * around an edge the way plain index arithmetic would.
		 */
		static int offsetIndex(
				const uint8_t index,
				const int fileDelta,
				const int rankDelta)
		{
			const int file = (index % 8) + fileDelta;
			const int rank = (index / 8) + rankDelta;

			if (file < 0 || file > 7 || rank < 0 || rank > 7)
			{
				return -1;
			}

			return rank * 8 + file;
		}

		static uint8_t encodePiece(const Piece& piece);
		static Piece decodePiece(uint8_t code);

		/**
		 * \brief Builds a \ref Board straight from the packed layout.
		 */
		static Board fromSquares(const std::array<uint8_t, 64>& squares)
		{
			return Board(squares);
		}

		static const std::array<uint8_t, 64>& squares(const Board& board);
		static std::array<uint8_t, 64>& mutableSquares(Board& board);

		/**
		 * \brief The value a king square hint holds while nothing is known
		 * about where that king stands.
		 *
		 * It is one past the last square of the board, so it can never be
		 * mistaken for a real one.
		 */
		static constexpr uint8_t UNKNOWN = 64;

		/**
		 * \brief The cached location of \p color's king, or \ref UNKNOWN.
		 * Only ever a hint - see \c Board::mKingSquareHint.
		 */
		static uint8_t kingSquareHint(const Board& board, Color color);
		static void setKingSquareHint(
				const Board& board, Color color, uint8_t index);

		/**
		 * \brief Whether \p code (as stored in the packed array) is a piece of
		 * the given colour.
		 *
		 * White pieces occupy the odd codes and black pieces the even ones, so
		 * the colour is the low bit of the zero-based code.
		 */
		static bool isColor(const uint8_t code, const Color color)
		{
			return code != 0
				&& (((code - 1) % 2 == 0)
						? Color::White
						: Color::Black) == color;
		}

		static PieceType typeOf(const uint8_t code)
		{
			return static_cast<PieceType>((code - 1) / 2);
		}
	};
}

#endif
