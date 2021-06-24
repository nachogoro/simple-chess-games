#ifndef GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041
#define GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041

#include "PlayedMove.h"
#include "Board.h"
#include "Piece.h"

#include <boost/optional.hpp>

#include <cstdint>
#include <vector>

namespace simplechess
{
	enum GameState
	{
		GAME_STATE_PLAYING,
		GAME_STATE_DRAWN,
		GAME_STATE_WHITE_WON,
		GAME_STATE_BLACK_WON
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
			 * \brief Factory method to create a new game from the standard
			 * starting position.
			 *
			 * \return The constructed Game.
			 */
			static Game createNewGame();

			/**
			 * \brief Factory method to create a new game from a given board
			 * position.
			 *
			 * The original position of the board is given as a string in
			 * Forsyth-Edwards Notation.
			 *
			 * \note FEN descriptions only give limited information about the
			 * history of the game. In particular, one cannot enforce certain
			 * drawing rules (50-move rule or the triplefold repetition), nor
			 * can it be inferred which pieces have been captured through the
			 * game. Hence, the history of the resulting \ref Game will not
			 * necessarily be of much use.
			 *
			 * \param fen The representation of the initial position in
			 * Forsyth-Edwards Notation.
			 *
			 * \return The constructed Game.
			 */
			static Game createGameFromStartingFen(const std::string& fen);

			/**
			 * \brief Returns the current state of the game.
			 *
			 * \return The current state of the game.
			 */
			GameState gameState() const;

			/**
			 * Returns the history of the game as a sequence of \ref GameStage.
			 *
			 * The latest reached position, along with the move which reached
			 * it, are the last element in the vector.
			 *
			 * \return The history of the game.
			 */
			const std::vector<GameStage>& gameHistory() const;

			/**
			 * \brief Returns the \ref Color of the player whose turn it is to
			 * play.
			 *
			 * \throws IllegalStateException if the game is finished.
			 *
			 * \return The player whose turn it is to play.
			 */
			Color currentPlayer() const;

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
			std::set<PieceMove> allAvailableMoves() const;

			/**
			 * \brief Make a move for the player whose turn it is to play.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   GAME_STATE_PLAYING).
			 * - The \p pieceMove is not a valid move for the current player.
			 *
			 * \param pieceMove The desired piece move.
			 * \param offerDraw \c true if the move is accompanied by an offer
			 * to draw to the opponent, \c false otherwise.
			 *
			 * \return A new copy of the Game in which the specified move has
			 * been played.
			 */
			Game makeMove(
					const PieceMove& pieceMove,
					bool offerDraw=false) const;

			/**
			 * \brief Whether the player whose turn it is to play can claim a
			 * draw.
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
			 * - If there is insufficient material for any side to checkmate.
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
			 *
			 * \return \c true if the current player can claim a draw, \c false
			 * otherwise.
			 */
			bool canClaimDraw() const;

			/**
			 * \brief Claim a draw.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   GAME_STATE_PLAYING).
			 * - The current player cannot claim a draw (\ref canClaimDraw() is
			 *   \c false)
			 *
			 * \return A new copy of the Game, identical to the current one but
			 * finished as a draw.
			 */
			Game claimDraw() const;

			/**
			 * \brief Resign the game.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   GAME_STATE_PLAYING).
			 *
			 * \return A new copy of the Game, identical to the current one but
			 * finished with the resignation of \p resigningPlayer.
			 */
			Game resign(Color resigningPlayer) const;

		private:
			const GameState mGameState;
			const std::vector<Step> mHistory;

	}
}

#endif
