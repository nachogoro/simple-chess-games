#ifndef GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041
#define GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041

#include "Move.h"
#include "Board.h"
#include "Piece.h"

#include <boost/optional.hpp>

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
			 * \return The constructed Game.
			 */
			static Game createGameFromStartingFen();

			/**
			 * \brief Returns the current state of the game.
			 *
			 * \return The current state of the game.
			 */
			GameState gameState() const;

			/**
			 * Returns the latest board position of the game.
			 *
			 * \return The latest board position of the game.
			 */
			const Board& currentPosition() const;

			/**
			 * \brief Returns the \ref Color of the player whose turn it is to
			 * play.
			 *
			 * \throws IllegalStateException if the game is finished.
			 *
			 * \return The player whose it is to play.
			 */
			Color currentPlayer() const;

			/**
			 * \brief Returns all the possible \c Move for a given piece in the
			 * board.
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
			std::set<Move> availableMovesForPiece(const Square& square) const;

			/**
			 * \brief Returns all available moves for the player whose turn it
			 * is to play.
			 *
			 * \return All the possible moves for the current player.
			 */
			std::set<Move> allAvailableMoves() const;

	}
}

#endif
