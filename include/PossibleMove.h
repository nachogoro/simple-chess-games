#ifndef POSSIBLEMOVE_H_85EFBDF8_15F3_4225_910A_00728C64D7B3
#define POSSIBLEMOVE_H_85EFBDF8_15F3_4225_910A_00728C64D7B3

#include "Move.h"
#include "Square.h"

namespace simplechess
{
	class PossibleMove
	{
		public:
			/**
			 * \brief Factory method to create a regular \c PossibleMove.
			 *
			 * A regular move is any move of pieces which is not a castle or a
			 * pawn promotion.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally.
			 * \param dstSquare The \ref Square to which the piece is moving.
			 * \return The constructed \c PossibleMove.
			 */
			static PossibleMove regularMove(
					const Square& srcSquare,
					const Square& dstSquare);

			/**
			 * \brief Factory method to create a pawn-promotion \c
			 * PossibleMove.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally.
			 * \param dstSquare The \ref Square to which the piece is moving.
			 * \return The constructed \c PossibleMove.
			 */
			static PossibleMove pawnPromotion(
					const Square& srcSquare,
					const Square& dstSquare);

			/**
			 * \brief Factory method to create a castling \c PossibleMove.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally. It must contain the king or a rook.
			 * \param castlingSide The side for the castling.
			 * \return The constructed \c PossibleMove.
			 */
			static PossibleMove castle(
					const Square& srcSquare,
					CastlingSide castlingSide);

			/**
			 * \brief Returns the string representation of the move in
			 * algebraic notation.
			 * \return The string representation of the move in algebraic
			 * notation.
			 */
			const std::string& inAlgebraicNotation() const;

			/**
			 * \brief Returns the type of the move.
			 * \return The type of the move.
			 */
			MoveType moveType() const;

			/**
			 * \brief Returns the original \ref Square of the moved piece.
			 *
			 * In the case of castling, this is the original square of the
			 * king.
			 *
			 * \return The original \c Square of the moved piece, if any piece
			 * is moved.
			 */
			const Square& originSquare() const;

			/**
			 * \brief Returns the final \ref Square of the moved piece, if
			 * any piece is moved.
			 *
			 * In the case of castling, this is the final square of the king.
			 *
			 * \return The final \c Square of the moved piece, if any piece is
			 * moved.
			 */
			const Square& finalSquare() const;

		private:
			PossibleMove(
					MoveType type,
					const Square& srcSquare,
					const Square& dstSquare);

			MoveType mType;
			Square mSrcSquare;
			Square mDstSquare;
			std::string mAlgebraicNotation;
	};
}

#endif
