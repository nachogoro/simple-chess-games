#ifndef BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017
#define BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017

#include <cpp/simplechess/Piece.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/Square.h>

#include <optional>

#include <array>
#include <cstdint>
#include <map>

namespace simplechess
{
	class BoardBuilder;
	struct BoardAccess;

	/**
	 * \brief A representation of a chess board.
	 *
	 * This representation is absent of any context beyond the position of the
	 * pieces on the board.
	 */
	class Board
	{
		public:
			/**
			 * \brief Returns the piece at the specified \p square or an empty
			 * optional if the \p square is empty.
			 *
			 * \param square The \ref Square being queried.
			 * \return The \c Piece at the specified \c Square or an empty
			 * optional if the square is empty.
			 */
			std::optional<Piece> pieceAt(const Square& square) const;

			/**
			 * \brief Returns a relation of all occupied squares and the pieces
			 * ocuppying them.
			 *
			 * \return A collection of all occupied squares.
			 */
			const std::map<Square, Piece>& occupiedSquares() const;

			Board(const Board& other);
			Board(Board&& other) noexcept;
			Board& operator=(const Board& other);
			Board& operator=(Board&& other) noexcept;
			~Board();

		private:
			friend class BoardBuilder;
			friend struct BoardAccess;

			/**
			 * \brief Constructor.
			 *
			 * Instantiates a \c Board from a series of \c Piece positions.
			 *
			 * \param piecePositions The positions of the pieces in the board.
			 */
			Board(const std::map<Square, Piece>& piecePositions);

			/**
			 * \brief Constructor from the packed representation.
			 *
			 * Lets code which already knows the layout of the board (FEN
			 * parsing, above all) build one without first assembling a map
			 * only for it to be taken apart again.
			 */
			explicit Board(const std::array<uint8_t, 64>& squares);

			/**
			 * \brief One entry per square, indexed by (rank - 1) * 8 + (file -
			 * 'a').
			 *
			 * A value of 0 means the square is empty; any other value encodes
			 * the piece occupying it. This is the authoritative state of the
			 * board: it is looked up in constant time and copied without
			 * allocating, both of which matter because generating the moves
			 * of a position copies the board once per candidate move.
			 */
			std::array<uint8_t, 64> mSquares;

			/**
			 * \brief Where each side's king was last known to stand, indexed
			 * by colour, or an index off the board while nothing is known.
			 *
			 * Locating the king is the first thing any check test does, and
			 * check tests run once per candidate move, so scanning the board
			 * for it every time is worth avoiding. This is only a hint:
			 * whoever reads it confirms the king is really there and falls
			 * back to a scan if it is not, which keeps it from ever being
			 * able to give a wrong answer.
			 */
			mutable std::array<uint8_t, 2> mKingSquareHint;

			/**
			 * \brief The same information as \ref mSquares, in the form \ref
			 * occupiedSquares() has to return.
			 *
			 * Built on first use rather than up front, since the boards
			 * created while searching for legal moves are never asked for it.
			 *
			 * \note Because this is filled in on demand, the first call to
			 * \ref occupiedSquares() on a given \c Board mutates it behind
			 * the \c const. The library is single-threaded: a \c Board must
			 * not be read from more than one thread at a time.
			 */
			mutable std::optional<std::map<Square, Piece>> mOccupiedSquares;
	};
}

#endif
