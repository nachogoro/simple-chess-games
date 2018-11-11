#ifndef MOVE_BEHAVIOUR_H_F16F14B1_D93F_47CF_A8C1_BE16067526C5
#define MOVE_BEHAVIOUR_H_F16F14B1_D93F_47CF_A8C1_BE16067526C5

#include "BoardImpl.h"

#include <Piece.h>
#include <Move.h>
#include <PossibleMove.h>
#include <Square.h>

#include <vector>

/**
 * \brief Abstract base class for the move behaviour of different pieces.
 * Every new piece type must provide its own implementation.
 */
namespace simplechess
{
	class MoveBehaviour
	{
		public:
			MoveBehaviour(PieceType type);
			PieceType pieceType() const;

			/**
			 * \brief Returns a list of all possible moves for the piece in
			 * srcSquare.
			 *
			 * The result might include actually invalid moves (those which
			 * would leave the player's king in check).
			 *
			 * \param srcSquare The position of the piece being queried.
			 * \param board The board being played.
			 * \param moveHistory All the moves made in the game.
			 * \return All the possible moves for the piece.
			 */
			virtual std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory) const = 0;

			/**
			 * \brief Whether the specified \a move is valid.
			 *
			 * It assumes that the pieces in the board are in a valid
			 * configuration already.
			 *
			 * The result might include actually invalid moves (those which
			 * would leave the player's king in check).
			 *
			 * \param move The move to be validated.
			 * \param board The board being played.
			 * \param moveHistory All the moves made in the game.
			 * \return Whether \a move is valid.
			 */
			virtual bool isValidMove(
					const Move& move,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory) const = 0;

			/**
			 * \brief Polymorphic clone method.
			 *
			 * It returns a dynamically-allocated copy of the current object.
			 *
			 * This is necessary for polymorphic copying of subclasses.
			 */
			virtual std::unique_ptr<MoveBehaviour> clone() const = 0;

		protected:
			/**
			 * \brief Whether the specified \a possibleMove is valid.
			 *
			 * This method is simply an adaptor for
			 * \ref isValidMove(const Move&, const BoardImpl&,
			 * const std::vector<Move>&).
			 *
			 * \param move The move to be validated.
			 * \param board The board being played.
			 * \param moveHistory All the moves made in the game.
			 * \return Whether \a move is valid.
			 */
			virtual bool isValidPossibleMove(
					const PossibleMove& possibleMove,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory) const;

		private:
			PieceType mPieceType;
	};
}

#endif
