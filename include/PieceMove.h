#ifndef PIECE_MOVE_H_565687CA_B90B_47F1_84E4_181A6503B9EE
#define PIECE_MOVE_H_565687CA_B90B_47F1_84E4_181A6503B9EE

namespace simplechess
{
	/**
	 * \brief Describes a single movement of a piece on a board.
	 *
	 * The travel is simply described by two squares: the square in which the
	 * piece was originally located and the square in which ended up.
	 *
	 * This description is independent of any board state, and it only
	 * represents a move of a piece in the abstract (without accounting for
	 * captures, checks, draw offers, etc.).
	 */
	class PieceMove
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * \param piece Piece being moved.
			 * \param src Original square of the piece.
			 * \param dst Final square of the piece.
			 */
			PieceMove(
					const Piece& piece,
					const Square& src,
					const Square& dst);

			/**
			 * \brief The piece whose movement is described.
			 * \return The piece whose movement is described.
			 */
			const Piece& piece() const;

			/**
			 * \brief Returns the original square of the moved piece.
			 * \return The original square of the moved piece.
			 */
			const Square& src() const;

			/**
			 * \brief Returns the final square of the moved piece.
			 * \return The final square of the moved piece.
			 */
			const Square& dst() const;

		private:
			const Piece mPiece;
			const Square mSrc;
			const Square mDst;
	};
}

#endif
