#ifndef MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC
#define MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC

#include "Board.h"
#include "Square.h"

#include <string>

namespace simplechess
{
	/**
	 * \brief The possible sides for a castling.
	 */
	enum CastlingSide
	{
		CASTLING_KING_SIDE,
		CASTLING_QUEEN_SIDE
	};

	/**
	 * \brief The different types of moves.
	 *
	 * Offering a draw is not considered a movement since it does not need to
	 * be accounted for in the score sheet.
	 */
	enum MoveType
	{
		MOVE_REGULAR,
		MOVE_PAWN_PROMOTION,
		MOVE_CASTLE_KING_SIDE,
		MOVE_CASTLE_QUEEN_SIDE,
		RESIGN,
		ACCEPT_DRAW
	};

	/**
	 * \brief A move made in a chess game.
	 */
	class Move
	{
		public:
			/**
			 * \brief Factory method to create a regular \c Move.
			 *
			 * A regular move is any move of pieces which is not a castle or a
			 * pawn promotion.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally.
			 * \param dstSquare The \ref Square to which the piece is moving.
			 * \param drawOffered Whether a draw is being offered to the
			 * oponent.
			 * \return The constructed \c Move.
			 */
			static Move regularMove(
					const Square& srcSquare,
					const Square& dstSquare,
					bool drawOffered);

			/**
			 * \brief Factory method to create a pawn-promotion \c Move.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally.
			 * \param dstSquare The \ref Square to which the piece is moving.
			 * \param promotedType The new type of the promoted pawn.
			 * \param drawOffered Whether a draw is being offered to the
			 * oponent.
			 * \return The constructed \c Move.
			 */
			static Move pawnPromotion(
					const Square& srcSquare,
					const Square& dstSquare,
					PieceType promotedType,
					bool drawOffered);

			/**
			 * \brief Factory method to create a castling \c Move.
			 *
			 * \param srcSquare The \ref Square in which the piece is
			 * originally.
			 * \param castlingSide The side for the castling.
			 * \param drawOffered Whether a draw is being offered to the
			 * oponent.
			 * \return The constructed \c Move.
			 */
			static Move castle(
					const Square& srcSquare,
					CastlingSide castlingSide,
					bool drawOffered);

			/**
			 * \brief Factory method to accept an oponent's draw.
			 *
			 * This move can only be played if the oponent has offered a draw
			 * in his last move.
			 *
			 * \return The constructed \c Move representing the draw.
			 */
			static Move acceptDraw();

			/**
			 * \brief Factory method to resign from the game.
			 *
			 * \return The constructed \c Move representing the resignation.
			 */
			static Move resign();

			/**
			 * \brief Factory method to create a \c Move from its string
			 * representation.
			 *
			 * \p algebraicNotation must be the move in algebraic notation.
			 *
			 * This method is more versatile than the factory methods which
			 * require a \ref Board, but offers less safety, since a move
			 * cannot be verified against the current state of the board.
			 *
			 * \throws Exception if \p algebraicNotation has invalid syntax.
			 *
			 * \param algebraicNotation A string representing a move in
			 * algebraic notation.
			 * \return The constructed \c Move.
			 */
			static Move instantiateFromString(
					const std::string& algebraicNotation);

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
			 * \brief Returns the original \ref Square of the moved piece, if
			 * any piece is moved.
			 *
			 * In the case of castling, this is the original square of the
			 * king.
			 *
			 * \return The original \c Square of the moved piece.
			 */
			const boost::optional<Square>& originSquare() const;

			/**
			 * \brief Returns the final \ref Square of the moved piece, if
			 * any piece is moved.
			 *
			 * In the case of castling, this is the final square of the king.
			 *
			 * \return The final \c Square of the moved piece.
			 */
			const boost::optional<Square>& finalSquare() const;

			/**
			 * \brief Returns the piece to which the pawn being moved is
			 * promoted.
			 *
			 * If this move does not involve a pawn being promoted, an empty
			 * optional is returned.
			 *
			 * \return The piece to which the pawn being moved is promoted or
			 * an empty optional if no pawn is being promoted in this move.
			 */
			const boost::optional<PieceType>& promoted() const;

			/**
			 * \brief Whether the player offers a draw with this move.
			 *
			 * If a draw is offered, the oponent is allowed to make an
			 * ACCEPT_DRAW move in the immediately next turn.
			 *
			 * \return \c true if a draw is being offered, \c false otherwise.
			 */
			bool isDrawOffered() const;

		private:
			Move(
					MoveType type,
					const boost::optional<Square>& srcSquare,
					const boost::optional<Square>& dstSquare,
					const boost::optional<PieceType>& promoted,
					bool drawOffered);

			MoveType mType;
			boost::optional<Square> mSrcSquare;
			boost::optional<Square> mDstSquare;
			boost::optional<PieceType> mPromoted;
			bool mDrawOffered;
			std::string mAlgebraicNotation;
	};
}

#endif
