#ifndef BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017
#define BOARD_H_060630C0_3ADF_4CCD_B96C_1728BF67C017

#include "Move.h"
#include "Piece.h"
#include "Square.h"

#include <boost/optional.hpp>

#include <map>

namespace simplechess
{
	/**
	 * \brief A representation of a chess board.
	 */
	class Board
	{
		public:
			/**
			 * Returns the board represented by the Forsyth-Edwards
			 * Notation string provided.
			 *
			 * \param fen The representation of the board in Forsyth-Edwards
			 * Notation.
			 * \return A \c Board equivalent to the provided \p fen
			 * description.
			 */
			static Board fromFen(const std::string& fen);

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
			 * \brief Returns the description of the state of the board in
			 * Forsyth-Edwards Notation.
			 * \return The description of the state of the board in FEN format.
			 */
			std::string fen() const;

		private:
			std::map<Square, Piece> mPiecePositions;
	};
}

#endif
