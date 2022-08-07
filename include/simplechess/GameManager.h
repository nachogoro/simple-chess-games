#ifndef GAME_MANAGER_H_83ECD5A4_C290_4A1B_8127_33357F92414E
#define GAME_MANAGER_H_83ECD5A4_C290_4A1B_8127_33357F92414E

#include <simplechess/Color.h>
#include <simplechess/Game.h>
#include <simplechess/PieceMove.h>

#include <optional>

#include <memory>
#include <string>

namespace simplechess
{
	class GameManagerImpl;

	/**
	 * \brief Entry point to the library.
	 *
	 * This class is responsible for creating new games and interacting with
	 * already-existing ones.
	 */
	class GameManager
	{
		public:
			/**
			 * \brief Constructor.
			 */
			GameManager();

			/**
			 * \brief Destructor.
			 */
			~GameManager();

			/**
			 * \brief Factory method to create a new game from the standard
			 * starting position.
			 *
			 * \return The constructed Game.
			 */
			Game createNewGame() const;

			/**
			 * \brief Factory method to create a new game from a given board
			 * position.
			 *
			 * The original position of the board is given as a string in
			 * Forsyth-Edwards Notation.
			 *
			 * \note FEN descriptions only give limited information about the
			 * history of the game. In particular, one cannot enforce certain
			 * drawing rules (triplefold repetition). Hence, the
			 * history of the resulting \ref Game will not necessarily be of
			 * much use.
			 *
			 * \throws std::invalid_argument if \p fen is not a valid FEN
			 * string.
			 *
			 * \param fen The representation of the initial position in
			 * Forsyth-Edwards Notation.
			 *
			 * \return The constructed Game.
			 */
			Game createGameFromFen(const std::string& fen) const;

			/**
			 * \brief Make a move for the player whose turn it is to play.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   Playing).
			 * - The \p pieceMove is not a valid move for the current player.
			 *
			 * \param game Game in which to move is to be made.
			 * \param pieceMove The desired piece move.
			 * \param offerDraw \c true if the move is accompanied by an offer
			 * to draw to the opponent, \c false otherwise.
			 *
			 * \return A new copy of the Game in which the specified move has
			 * been played.
			 */
			Game makeMove(
					const Game& game,
					const PieceMove& move,
					bool offerDraw=false) const;

			/**
			 * \brief Claim a draw.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   Playing).
			 * - The current player cannot claim a draw (\ref
			 *   Game.reasonToClaimDraw() is empty)
			 *
			 * \param game Game to be drawn.
			 * \return A new copy of the Game, identical to the current one but
			 * finished as a draw.
			 */
			Game claimDraw(const Game& game) const;

			/**
			 * \brief Resign the game.
			 *
			 * \throws IllegalStateException in the following circumstances:
			 * - The Game has already concluded (its state is not
			 *   Playing).
			 *
			 * \param game Game to be resigned.
			 * \return A new copy of the Game, identical to the current one but
			 * finished with the resignation of \p resigningPlayer.
			 */
			Game resign(const Game& game, Color resigningPlayer) const;

		private:
			std::unique_ptr<GameManagerImpl> mImpl;
	};
};

#endif
