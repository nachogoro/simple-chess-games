#ifndef GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041
#define GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041

#include <Exceptions.h>
#include <GameStage.h>
#include <PieceMove.h>
#include <Square.h>

#include <boost/optional.hpp>

#include <set>
#include <vector>

namespace simplechess
{
	/**
	 * \brief The state of a game.
	 */
	enum GameState
	{
		/**
		 * \brief The game is still being played.
		 */
		GAME_STATE_PLAYING,

		/**
		 * \brief The game ended in a draw.
		 */
		GAME_STATE_DRAWN,

		/**
		  \brief The game ended with a victory for white.
		 */
		GAME_STATE_WHITE_WON,

		/**
		 * \brief The game ended with a victory for black.
		 */
		GAME_STATE_BLACK_WON
	};

	/**
	 * \brief Reasons why a game might be drawn.
	 */
	enum DrawReason
	{
		/**
		 * \brief Stalemate (the active side has no valid moves and is not in
		 * check).
		 */
		DRAW_REASON_STALEMATE,

		/**
		 * \brief Neither side has sufficient material to mate the other.
		 */
		DRAW_REASON_INSUFFICIENT_MATERIAL,

		/**
		 * \brief A side offered a draw and it was accepted.
		 */
		DRAW_REASON_OFFERED_AND_ACCEPTED,

		/**
		 * \brief The same position has been reached 3 times or will be
		 * reached for the thrid time with the next move.
		 *
		 * \note See FIDE rule 9.2.1 * and 9.2.2.
		 */
		DRAW_REASON_THREE_FOLD_REPETITION,

		/**
		 * \brief The same position has been reached 5 times.
		 *
		 * \note See FIDE rule 9.6.1.
		 */
		DRAW_REASON_FIVE_FOLD_REPETITION,

		/**
		 * \brief At least fifty full moves (i.e. each side has played their
		 * move) have been played (or will have been played after next move)
		 * since the last capture or pawn move.
		 *
		 * TODO cover the case in which the 50th is not yet made
		 *
		 * \note See FIDE rules 9.3.1 and 9.3.2.
		 */
		DRAW_REASON_50_MOVE_RULE,

		/**
		 * \brief At least fifty full moves (i.e. each side has played their
		 * move) have been played (or will have been played after next move)
		 * since the last capture or pawn move.
		 *
		 * TODO test the case in which the 75th is checkmate.
		 *
		 * \note See FIDE rule 9.6.2.
		 */
		DRAW_REASON_75_MOVE_RULE
	};

	class GameBuilder;

	/**
	 * \brief A representation of a game of chess at a given point.
	 *
	 * The class is immutable, so all methods which would change its state
	 * instead return a new copy of the object with the updated state.
	 */
	class Game
	{
		public:
			/**
			 * \brief Returns the current state of the game.
			 *
			 * \return The current state of the game.
			 */
			GameState gameState() const;

			/**
			 * \brief Returns the reason why the game ended in a draw.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has not been drawn (its state is not
			 *   GAME_STATE_DRAWN).
			 *
			 * \return The reason why the game was drawn.
			 */
			DrawReason drawReason() const;

			/**
			 * \brief Returns the history of the game as a sequence of \ref
			 * GameStage.
			 *
			 * The latest reached position, along with the move which reached
			 * it, are the last element in the vector.
			 *
			 * \note The most reliable way of getting the move counter of a
			 * certain stage is not to rely on the position on this vector but
			 * instead use the appropriate method in \ref GameStage.
			 *
			 * \return The history of the game.
			 */
			const std::vector<GameStage>& history() const;

			/**
			 * \brief Returns the latest stage of the game.
			 *
			 * This is a convenience method to access the last element of \ref
			 * history().
			 *
			 * \return The latest stage of the game.
			 */
			const GameStage& currentStage() const;

			/**
			 * \brief Returns the \ref Color which is to move next.
			 *
			 * \return The \ref Color which is to move next.
			 */
			Color activeColor() const;

			/**
			 * \brief Returns all the possible \c PieceMove for a given piece
			 * in the board.
			 *
			 * If the square is empty or does not contain a piece of the player
			 * whose turn it is to play, an empty set is returned.
			 *
			 * \note Castling is considered a King's move, so it's only listed
			 * as such.
			 *
			 * \param square The \ref Square whose piece is being queried.
			 * \return All the possible moves for the piece.
			 */
			std::set<PieceMove> availableMovesForPiece(const Square& square) const;

			/**
			 * \brief Returns all available moves for the player whose turn it
			 * is to play.
			 *
			 * \return All the possible moves for the current player.
			 */
			const std::set<PieceMove>& allAvailableMoves() const;

			/**
			 * \brief Returns an optional value containing the reason under
			 * which the current player can claim a draw.  If a draw cannot be
			 * claimed, the optional value is empty.
			 *
			 * A draw can be claimed under the following circumstances:
			 * - If the opponent offered a draw in its immediately previous
			 *   move.
			 * - If the current position has occurred at least three times in
			 *   the game (defined as: the same pieces occupy the same squares,
			 *   the same player has the move, the remaining castling rights
			 *   are the same and the possibility to capture en passant is the
			 *   same). In this case, the draw can be claimed right before it
			 *   appears for the third time or when it has just appeared.
			 * - If no captures or pawn moves have occurred in the last fifty
			 *   moves.
			 *
			 * \note The game automatically ends in a draw, without any player
			 * having to claim it in the following circumstances:
			 * - Stalemate (the current player is not in check but has no valid
			 *   moves).
			 * - Fivefold repetition (the same position has been reached five
			 *   times in the game).
			 * - If no captures or pawn moves have occurred in the last
			 *   seventy-five moves (unless the last move results in
			 *   checkmate).
			 * - If there is insufficient material for any side to checkmate.
			 *
			 * \throws \ref IllegalStateException if the state of the game is
			 * not \ref GAME_STATE_PLAYING.
			 *
			 * \return A possible reason to claim a draw if it exists, an empty
			 * value otherwise.
			 */
			const boost::optional<DrawReason>& reasonToClaimDraw() const;

		private:
			friend class GameBuilder;

			Game(
					GameState gameState,
					const boost::optional<DrawReason>& drawReason,
					const std::vector<GameStage>& history,
					const std::set<PieceMove>& allAvailableMoves,
					const boost::optional<DrawReason>& reasonToClaimDraw);

			GameState mGameState;
			boost::optional<DrawReason> mReasonGameWasDrawn;
			std::vector<GameStage> mHistory;
			std::set<PieceMove> mAllAvailableMoves;
			boost::optional<DrawReason> mReasonToClaimDraw;
	};
}

#endif
