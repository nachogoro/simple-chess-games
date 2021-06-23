#ifndef MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC
#define MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC

#include "Board.h"
#include "PieceMove.h"
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
	 */
	enum MoveType
	{
		MOVE_PIECE,
		ACCEPT_DRAW,
		RESIGN_WHITE,
		RESIGN_BLACK
	};

	/**
	 * \brief A move in a chess game.
	 *
	 * A move can only be made by the player whose turn it is to play.
	 *
	 * Based in FIDE rule §9.1, draws should be offered by the proposing side
	 * at the time of making their move but before hitting the clock. While
	 * they can be made before making the move, the other side may ask them to
	 * make the move first before considering the offer. The other side may
	 * accept the draw once it's their turn, or reject it either verbally or by
	 * simply making a regular move.
	 *
	 * Therefore, this library does not consider draw offers as moves in and of
	 * themselves, but as an addendum to a regular move. Accepting a draw is
	 * considered a move in and of itself, and can therefore only be performed
	 * by the player whose turn it is to play. As per FIDE rules, a draw offer
	 * cannot be retracted, and is only valid for the next half turn.
	 *
	 * Resignations are considered a special type of move. According to FIDE
	 * rules, a player may resign at any given point in a game, regardless of
	 * whether it is their turn or not. They are considered a move for the
	 * purpose of this library, as they need to be recorded in the list of
	 * moves. However, as a player can resign during the other player's turn,
	 * the color of the resigning player needs to be specified when resigning.
	 *
	 * It is important to ensure that the applications making use of this
	 * library do not allow a player to resign as the opposite player.
	 *
	 * \note In order to create a move, one needs to know the state of the
	 * board at that point, as it is otherwise impossible to remove
	 * ambiguities. For example, in algebraic notation "xe4" means "A pawn
	 * takes on e4", but without knowing the state of the board when the move
	 * is made it is impossible to know if the capturing pawn was originally in
	 * d5, f5, d4 or f4 (in the case of en passant); nor the square where it
	 * ended (e4 or e3 in the case of en passant).
	 */
	class Move
	{
		public:
			/**
			 * \brief Factory method to create a regular \c Move.
			 *
			 * A regular move is any move of pieces which is not a pawn
			 * promotion nor the acceptance of a draw.
			 *
			 * \note In the case of castling, \p move should refer to the
			 * movement of the King.
			 *
			 * \throws std::invalid_argument in the following
			 * circumstances:
			 * - The original square in \p move does not contain a piece of the
			 *   color of the current player.
			 * - The final square in \p move is not reachable by the piece in
			 *   the original square.
			 * - The move causes a pawn to reach the last rank (in which case
			 *   it must be promoted and a different API should be used).
			 * - The move is invalid any other way (e.g. it leaves the own king
			 *   in check).
			 *
			 * \param board The current state of the board.
			 * \param move The movement of the piece.
			 * \param drawOffered Whether a draw is being offered to the
			 * oponent.
			 * \return The constructed \c Move.
			 */
			static Move regularMove(
					const Board& board,
					const PieceMove& move,
					bool drawOffered=false);

			/**
			 * \brief Factory method to create a pawn-promotion \c Move.
			 *
			 * \throws std::invalid_argument in the following
			 * circumstances:
			 * - The original square in \p move does not contain a pawn of the
			 *   color of the current player.
			 * - The final square in \p move is not reachable by the pawn in
			 *   the original square, or is not the last rank.
			 * - \p promotedType is not one of \ref TYPE_ROOK, \ref
			 *   TYPE_BISHOP, \ref TYPE_KNIGHT or \ref TYPE_QUEEN.
			 * - The move is invalid any other way (e.g. it leaves the own king
			 *   in check).
			 *
			 * \param board The current state of the board.
			 * \param move The movement of the pawn.
			 * \param promotedType The new type of the promoted pawn.
			 * \param drawOffered Whether a draw is being offered to the
			 * oponent.
			 * \return The constructed \c Move.
			 */
			static Move pawnPromotion(
					const Board& board,
					const PieceMove& move,
					PieceType promotedType,
					bool drawOffered=false);

			/**
			 * \brief Factory method to create a move which draws the game.
			 *
			 * A draw can be forced in the following circumstances:
			 * - The opponent offered a draw in the previous move.
			 * - A position can be reached for the 3rd time or more from the
			 *   current position (FIDE §9.2.1 and §9.2.2)
			 * - No capture has been made and no pawn has been moved in the
			 *   last fifty moves (FIDE §9.3.1 and §9.3.1).
			 *
			 * \throws std::invalid_argument if none of the previous
			 * circumstances concur.
			 *
			 * \param board The current state of the board.
			 * \return The constructed \c Move.
			 */
			static Move drawGame(const Board& board);

			/**
			 * \brief Factory method to create a resignation move from \p
			 * color.
			 *
			 * \return The constructed \c Move.
			 */
			static Move resign(Color color);

			/**
			 * \brief Factory method to create a \c Move from its string
			 * representation.
			 *
			 * \p algebraicNotation must be the move in algebraic notation.
			 *
			 * \throws std::invalid_argument in the following circumstances:
			 * - \p algebraicNotation has invalid syntax.
			 * - \p algebraicNotation represents an invalid move according to
			 *   the current state of the board.
			 *
			 * \param board The current state of the board.
			 * \param algebraicNotation A string representing a move in
			 * algebraic notation.
			 * \return The constructed \c Move.
			 */
			static Move fromString(
					const Board& board,
					const std::string& algebraicNotation);

			/**
			 * \brief Returns the string representation of the move in
			 * algebraic notation.
			 * \return The string representation of the move in algebraic
			 * notation.
			 */
			const std::string& inAlgebraicNotation() const;

			/**
			 * \brief The type of the move.
			 *
			 * \return The type of the move.
			 */
			MoveType moveType() const;

			/**
			 * \brief Returns the movement of the moved piece, if any piece has
			 * been moved.
			 *
			 * \note In the case of castling, this describes the movement of
			 * the King.
			 *
			 * \return The movement of the moved piece.
			 */
			const boost::optional<PieceMove>& pieceMove() const;

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

			/**
			 * \brief Whether a piece has been captured by this move.
			 *
			 * \return \c true if an opponent's piece has been captured in this
			 * move, \c false otherwise.
			 */
			bool isPieceCaptured() const;

			/**
			 * \brief Whether the move has resulted in a check.
			 *
			 * \note For moves which don't involve moving pieces, this method
			 * returns \c false.
			 *
			 * \note A checkmate is a type of check. Hence, this method will
			 * return \c true for a checkmate.
			 *
			 * \return \c true if the move has resulted in a check, \c false
			 * otherwise.
			 */
			bool isCheck() const;

			/**
			 * \brief Whether the move has resulted in checkmate.
			 *
			 * \note For moves which don't involve moving pieces, this method
			 * returns \c false.
			 *
			 * \return \c true if the move has resulted in checkmate, \c false
			 * otherwise.
			 */
			bool isCheckmate() const;

		private:
			Move(
					const Board& board,
					MoveType type,
					const boost::optional<PieceMove>& pieceMove,
					const boost::optional<PieceType>& promoted,
					bool drawOffered);

			MoveType mType;
			boost::optional<PieceMove> mPieceMove;
			boost::optional<PieceType> mPromoted;
			bool mDrawOffered;
			std::string mAlgebraicNotation;
	};
}

#endif
