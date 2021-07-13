#ifndef BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017
#define BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017

#include "Piece.h"
#include "PieceMove.h"
#include "Square.h"

#include <boost/optional.hpp>

#include <map>
#include <vector>

namespace simplechess
{
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
			 * \brief Constructor.
			 *
			 * Instantiates a \c Board from a series of \c Piece positions.
			 *
			 * \param piecePositions The positions of the pieces in the board.
			 */
			Board(const std::map<Square, Piece>& piecePositions);

			/**
			 * \brief Returns the piece at the specified \p square or an empty
			 * optional if the \p square is empty.
			 *
			 * \param square The \ref Square being queried.
			 * \return The \c Piece at the specified \c Square or an empty
			 * optional if the square is empty.
			 */
			boost::optional<Piece> pieceAt(const Square& square) const;

			/**
			 * \brief Returns a relation of all occupied squares and the pieces
			 * ocuppying them.
			 *
			 * \return A collection of all occupied squares.
			 */
			const std::map<Square, Piece>& occupiedSquares() const;

		private:
			std::map<Square, Piece> mPiecePositions;
	};
}

#endif
