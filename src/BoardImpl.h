#ifndef BOARDIMPL_H_90F11305_80F1_42AD_A7FA_63C36194E4B1
#define BOARDIMPL_H_90F11305_80F1_42AD_A7FA_63C36194E4B1

#include "PieceImpl.h"

#include <Board.h>

namespace simplechess
{
	class BoardImpl
	{
		public:
			/**
			 * Returns the default chess board, before any piece is moved.
			 */
			static BoardImpl createDefaultBoard();

			BoardImpl(const std::map<Square, PieceImpl>& piecePositions);

			/**
			 * Returns the piece at \a square.
			 *
			 * \throws Exception if \a square is empty.
			 *
			 * \return The piece at the given square.
			 */
			PieceImpl& pieceAt(const Square& square);

			/**
			 * Returns the piece at \a square.
			 *
			 * \throws Exception if \a square is empty.
			 *
			 * \return The piece at the given square.
			 */
			const PieceImpl& pieceAt(const Square& square) const;


			/**
			 * \brief Returns a representation of the board as a \ref Board
			 * object.
			 * \return A representation of the board as a \c Board object.
			 */
			Board toPublicBoard() const;

			/**
			 * \brief Performs the \a move in the current board.
			 *
			 * It does not check for the validity of the move.
			 *
			 * \param move The move to be made.
			 */
			void makeMove(const Move& move) const;

			/**
			 * Whether square is not occupied by any piece.
			 */
			bool isEmpty(const Square& square) const;

			/**
			 * Whether a piece of color color could move into dstSquare.
			 */
			bool isOccupiableBy(
					const Square& dstSquare, PieceColor color) const;

			/**
			 * Whether dstSquare is occupied by a piece of color color.
			 */
			bool isOccupiedByPieceOfColor(
					const Square& dstSquare, PieceColor color) const;

			/**
			 * Whether both squares are in the same diagonal.
			 */
			bool isInDiagonal(
					const Square& src, const Square& dst) const;

			/**
			 * Whether both squares share rank or file.
			 */
			bool isInSameRankOrFile(const Square& src, const Square& dst) const;

			/**
			 * Whether there is clean line of sight between src and dst,
			 * whether horizontal, vertical or diagonal.
			 */
			bool isReachable(const Square& src, const Square& dst) const;

			/**
			 * Returns the position of the king of the specified color.
			 */
			const Square& kingSquare(PieceColor color) const;

			/**
			 * Whether target is threatened by any piece of color attackColor.
			 */
			bool isThreatenedBy(
					const Square& target, PieceColor attackColor) const;

		private:
			std::map<Square, PieceImpl> mPiecePositions;

	};
}

#endif
