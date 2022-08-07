#ifndef BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017
#define BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017

#include <simplechess/Piece.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <optional>

#include <map>

namespace simplechess
{
	class BoardBuilder;

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

		private:
			friend class BoardBuilder;

			/**
			 * \brief Constructor.
			 *
			 * Instantiates a \c Board from a series of \c Piece positions.
			 *
			 * \param piecePositions The positions of the pieces in the board.
			 */
			Board(const std::map<Square, Piece>& piecePositions);

			std::map<Square, Piece> mPiecePositions;
	};
}

#endif
