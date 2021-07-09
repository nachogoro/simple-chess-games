#ifndef PIECE_MOVE_H_565687CA_B90B_47F1_84E4_181A6503B9EE
#define PIECE_MOVE_H_565687CA_B90B_47F1_84E4_181A6503B9EE

#include <Piece.h>
#include <Square.h>

#include <boost/optional.hpp>
#include <string>

namespace simplechess
{
	/**
	 * \brief Describes a move which can be made by a player.
	 *
	 * The move is simply described by the piece which moves, two squares (the
	 * square in which the piece was originally located and the square in which
	 * ended up) and, in the case of pawn promotions, the piece to which it
	 * promoted.
	 *
	 * This description is independent of any board state, and it only
	 * represents a move of a piece in the abstract (e.g. "The knight on g1
	 * moved to f3"), without accounting for captures, checks, draw offers,
	 * etc.
	 */
	class PieceMove
	{
		public:
			/**
			 * \brief Factory method to create a regular \c PieceMove.
			 *
			 * A regular move is any move of pieces which is not a pawn
			 * promotion.
			 *
			 * \note In the case of castling, \p src should refer to the
			 * original square of the King.
			 *
			 * \note In the case of castling, \p dst should refer to the
			 * final square of the King.
			 *
			 * \param piece Piece being moved.
			 * \param src Original square of the piece.
			 * \param dst Final square of the piece.
			 *
			 * \return The constructed \c PieceMove.
			 */
			static PieceMove regularMove(
					const Piece& piece,
					const Square& src,
					const Square& dst);

			/**
			 * \brief Factory method to create a pawn-promotion \c PieceMove.
			 *
			 * \throws std::invalid_argument in the following
			 * circumstances:
			 * - \p piece is not a pawn.
			 * - \p promotedType is not one of \ref TYPE_ROOK, \ref
			 *   TYPE_BISHOP, \ref TYPE_KNIGHT or \ref TYPE_QUEEN.
			 *
			 * \param piece Piece being moved (must be of type \ref TYPE_PAWN).
			 * \param src Original square of the pawn.
			 * \param dst Final square of the pawn.
			 * \param promotedType The new type of the promoted pawn.
			 * \return The constructed \c PieceMove.
			 */
			static PieceMove pawnPromotion(
					const Piece& piece,
					const Square& src,
					const Square& dst,
					PieceType promotedType);

			/**
			 * \brief The piece whose movement is described.
			 * \return The piece whose movement is described.
			 */
			const Piece& piece() const;

			/**
			 * \brief Returns the original square of the moved piece.
			 *
			 * \note In the case of castling, it refers to the original square
			 * of the King.
			 *
			 * \return The original square of the moved piece.
			 */
			const Square& src() const;

			/**
			 * \brief Returns the final square of the moved piece.
			 *
			 * \note In the case of castling, it refers to the final square of
			 * the King.
			 *
			 * \return The final square of the moved piece.
			 */
			const Square& dst() const;

			/**
			 * \brief Returns the new type of the promoted pawn.
			 *
			 * If this move does not involve a pawn being promoted, an empty
			 * optional is returned.
			 *
			 * \return The piece type to which the pawn being moved is promoted
			 * or an empty optional if no pawn is being promoted in this move.
			 */
			const boost::optional<PieceType>& promoted() const;

			bool operator<(const PieceMove& o) const;

		private:
			PieceMove(
					const Piece& piece,
					const Square& src,
					const Square& dst,
					const boost::optional<PieceType>& promotedType);

			Piece mPiece;
			Square mSrc;
			Square mDst;
			boost::optional<PieceType> mPromotedType;
	};
}

#endif
