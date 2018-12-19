#ifndef GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041
#define GAME_H_AA82C7D6_D956_405F_95B0_8A23678A5041

#include "Move.h"
#include "Board.h"
#include "Piece.h"

#include <boost/optional.hpp>

#include <vector>

namespace simplechess
{
	/**
	 * \brief The possible endings of a chess game.
	 */
	enum GameEnding
	{
		ENDING_WHITE_WINS,
		ENDING_BLACK_WINS,
		ENDING_DRAW
	};

	/**
	 * \brief A representation of a chess game at a given moment.
	 */
	class Game
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * \param id A unique ID for the \c Game.
			 * \param board The current state of the board.
			 * \param moveHistory All the moves made in the game until now.
			 * \param capturedPieces All the captured pieces in the game.
			 */
			Game(
					const uint32_t id,
					const Board& board,
					const std::vector<Move>& moveHistory,
					const std::vector<Piece>& capturedPieces);

			/**
			 * \brief Returns the ID of the \c Game, which identifies it before
			 * \ref GameEngine.
			 *
			 * \return The ID of the \c Game.
			 */
			uint32_t id() const;

			/**
			 * \brief Returns the current state of the \ref Board.
			 * \return The current state of the \c Board.
			 */
			const Board& board() const;

			/**
			 * \brief Returns the winner of the game.
			 *
			 * If the game is not over, an empty optional is returned.
			 *
			 * \return The winner of the fame or an empty optional if the game
			 * is not over.
			 */
			boost::optional<GameEnding> gameWinner() const;

			/**
			 * \brief Returns the next color to make a move.
			 *
			 * If the game is over, an empty optional is returned.
			 *
			 * \return The next color to make a move or an empty optional if
			 * the game is over.
			 */
			boost::optional<Color> nextPlayer() const;

			/**
			 * \brief Returns all the moves made in the game.
			 * \return All the moves made in the game.
			 */
			const std::vector<Move>& moveHistory() const;

			/**
			 * \brief Returns all the captured pieces in the game.
			 * \return All the captured pieces in the game.
			 */
			const std::vector<Piece>& capturedPieces() const;

		private:
			uint32_t mId;
			Board mBoard;
			std::vector<Move> mMoveHistory;
			std::vector<Piece> mCapturedPieces;
	};
};

#endif
