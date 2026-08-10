#ifndef GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041
#define GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041

#include <simplechess/Exceptions.h>
#include <simplechess/GameStage.h>
#include <simplechess/PieceMove.h>
#include <simplechess/PlayedMove.h>
#include <simplechess/Square.h>

#include <optional>

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace simplechess
{
	/**
	 * \brief The state of a game.
	 */
	enum class GameState
	{
		/**
		 * \brief The game is still being played.
		 */
		Playing,

		/**
		 * \brief The game ended in a draw.
		 */
		Drawn,

		/**
		  \brief The game ended with a victory for white.
		 */
		WhiteWon,

		/**
		 * \brief The game ended with a victory for black.
		 */
		BlackWon
	};

	/**
	 * \brief Reasons why a game might be drawn.
	 */
	enum class DrawReason
	{
		/**
		 * \brief Stalemate (the active side has no valid moves and is not in
		 * check).
		 */
		StaleMate,

		/**
		 * \brief Neither side has sufficient material to mate the other.
		 */
		InsufficientMaterial,

		/**
		 * \brief A side offered a draw and it was accepted.
		 */
		OfferedAndAccepted,

		/**
		 * \brief The same position has been reached 3 times or will be
		 * reached for the thrid time with the next move.
		 *
		 * \note See FIDE rule 9.2.1 * and 9.2.2.
		 */
		ThreeFoldRepetition,

		/**
		 * \brief The same position has been reached 5 times.
		 *
		 * \note See FIDE rule 9.6.1.
		 */
		FiveFoldRepetition,

		/**
		 * \brief At least fifty full moves (i.e. each side has played their
		 * move) have been played (or will have been played after next move)
		 * since the last capture or pawn move.
		 *
		 * \note See FIDE rules 9.3.1 and 9.3.2.
		 */
		FiftyMoveRule,

		/**
		 * \brief At least seventy-five full moves (i.e. each side has played their
		 * move) have been played since the last capture or pawn move.
		 *
		 * \note See FIDE rule 9.6.2. If the 75th move results in checkmate,
		 * the checkmate takes precedence over the draw rule.
		 */
		SeventyFiveMoveRule,

		/**
		 * \brief The opponent (inactive side) has only a king remaining,
		 * making it impossible for them to win. The active side may claim
		 * a draw. This is never automatically enforced.
		 */
		OpponentInsufficientMaterial
	};

	/**
	 * \brief Controls whether mandatory draw rules (fivefold repetition,
	 * 75-move rule, insufficient material) are automatically enforced by
	 * the library, or whether they only become claimable.
	 *
	 * \note Stalemate and checkmate are always enforced regardless of
	 * this setting, as they are fundamental game-ending conditions.
	 */
	enum class DrawEnforcement
	{
		/**
		 * \brief The library automatically ends the game in a draw when
		 * mandatory FIDE draw conditions are met (default behaviour).
		 */
		Automatic,

		/**
		 * \brief Mandatory FIDE draw conditions are not automatically
		 * enforced; they become claimable instead.
		 */
		ClaimOnly
	};

	class GameBuilder;

	/**
	 * \brief One position in the history of a game, together with the move
	 * which was played from it.
	 */
	struct HistoryEntry
	{
		/**
		 * \brief The position the move was played from.
		 */
		GameStage stage;

		/**
		 * \brief The move played from \ref stage, which leads to the
		 * position of the next entry.
		 */
		PlayedMove move;
	};

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
			 * \brief Returns the reason why the game ended in a draw, or an
			 * empty optional if it did not end in a draw.
			 *
			 * \return The reason why the game was drawn.
			 */
			const std::optional<DrawReason>& drawReason() const;

			/**
			 * \brief Returns the history of the game.
			 *
			 * Each \ref HistoryEntry holds a position and the move made FROM
			 * that position, which transitions to the next one. The vector
			 * excludes the current stage of the game (as no move has been
			 * played yet from the current position).
			 *
			 * \return The history of the game.
			 */
			const std::vector<HistoryEntry>& history() const;

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
			 * whose turn it is to play, an empty vector is returned.
			 *
			 * \note Castling is considered a King's move, so it's only listed
			 * as such.
			 *
			 * \param square The \ref Square whose piece is being queried.
			 * \return All the possible moves for the piece.
			 */
			std::vector<PieceMove> availableMovesForPiece(const Square& square) const;

			/**
			 * \brief Returns all available moves for the player whose turn it
			 * is to play.
			 *
			 * The moves are ordered by \ref PieceMove::operator<, and contain
			 * no duplicates.
			 *
			 * \return All the possible moves for the current player.
			 */
			const std::vector<PieceMove>& allAvailableMoves() const;

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
			 * \note A finished game has no reason to claim a draw, so the
			 * optional is empty for one.
			 *
			 * \return A possible reason to claim a draw if it exists, an empty
			 * value otherwise.
			 */
			const std::optional<DrawReason>& reasonToClaimDraw() const;

			/**
			 * \brief Returns the draw enforcement mode of this game.
			 *
			 * \return The \ref DrawEnforcement mode.
			 */
			DrawEnforcement drawEnforcement() const;

		private:
			friend class GameBuilder;

			Game(
					GameState gameState,
					const std::optional<DrawReason>& drawReason,
					std::vector<HistoryEntry> history,
					GameStage currentStage,
					std::vector<PieceMove> allAvailableMoves,
					const std::optional<DrawReason>& reasonToClaimDraw,
					DrawEnforcement drawEnforcement,
					std::map<std::string, uint8_t> previouslyReachedPositions);

			/**
			 * \brief The number of times each position in \ref history() has
			 * been reached, keyed by the position's n-fold repetition key.
			 *
			 * Maintained incrementally as moves are played rather than
			 * recomputed from the history, and cleared whenever a move makes
			 * every earlier position unreachable (a capture, a pawn advance
			 * or the loss of a castling right), which also keeps it small.
			 */
			const std::map<std::string, uint8_t>&
				previouslyReachedPositions() const;

			GameState mGameState;
			std::optional<DrawReason> mReasonGameWasDrawn;
			std::vector<HistoryEntry> mHistory;
			GameStage mCurrentStage;
			std::vector<PieceMove> mAllAvailableMoves;
			std::optional<DrawReason> mReasonToClaimDraw;
			DrawEnforcement mDrawEnforcement;
			std::map<std::string, uint8_t> mPreviouslyReachedPositions;
	};
}

#endif
