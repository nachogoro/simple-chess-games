#ifndef GAMEENGINE_H_4D52E7AA_7E06_4253_859C_DCC7B196EAC5
#define GAMEENGINE_H_64D52E7AA_7E06_4253_859C_DCC7B196EAC5

#include "Game.h"
#include "Move.h"
#include "PossibleMove.h"
#include "Square.h"

#include <memory>

namespace simplechess
{
	class GameEngineImpl;

	/**
	 * \brief Class to manage all games.
	 */
	class GameEngine
	{
		public:
			/**
			 * \brief Default constructor.
			 */
			GameEngine();

			/**
			 * \brief Creates a new \ref Game from the algebraic transcript in
			 * \a inputFile.
			 *
			 * \throws Exception if \a inputFile contains invalid syntax/moves.
			 *
			 * \param inputFile File containing the algebraic transcript of the
			 * game.
			 *
			 * \return The newly created \c Game.
			 */
			Game createGameFromFile(const std::string& inputFile);

			/**
			 * \brief Creates a new \ref Game ready to be played.
			 *
			 * \return The newly created \c Game.
			 */
			Game createNewGame();

			/**
			 * \brief Deletes the \ref Game identified by \a id.
			 *
			 * \throws Exception if \a id does not identify a \c Game.
			 *
			 * \param id The ID of the \c Game.
			 */
			void deleteGame(uint32_t id);

			/**
			 * \brief Exports the \ref Game identified by \a id to file \a
			 * outFile in algebraic notation.
			 *
			 * \param id The ID of the \c Game.
			 * \param outFile The path to the file which will contain the game
			 * transcript.
			 */
			void exportGame(uint32_t id, const std::string& outFile) const;

			/**
			 * \brief Returns the possible moves that can be made by the piece
			 * in \a srcSquare inside the \ref Game identified by \a id.
			 *
			 * \throws Exception if \a srcSquare does not contain a piece of
			 * the current player.
			 *
			 * \param id The ID of the \c Game.
			 * \param srcSquare The square containing the queried piece.
			 *
			 * \return All the possible moves of the piece.
			 */
			std::vector<PossibleMove> possibleMoves(
					uint32_t id, const Square& srcSquare) const;

			/**
			 * \brief Makes a \a move in the \ref Game identified by \a id.
			 *
			 * \throws Exception if the \a move is not a valid move for the
			 * current player.
			 *
			 * \param id The ID of the \c Game.
			 * \param move The move to be made.
			 *
			 * \return The new state of the \c Game.
			 */
			Game makeMove(uint32_t id, const Move& move);

		private:
			std::unique_ptr<GameEngineImpl> mImpl;
	};
};

#endif
