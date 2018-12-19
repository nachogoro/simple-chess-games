#ifndef MOVE_HISTORY_H_6F6DA4E1_36A3_4F9D_9123_6BA3E98D3D47
#define MOVE_HISTORY_H_6F6DA4E1_36A3_4F9D_9123_6BA3E98D3D47

#include <Board.h>
#include <Move.h>
#include <Square.h>

#include <vector>

namespace simplechess
{
	/**
	 * \brief Class representing all the moves made in a game.
	 */
	class MoveHistory
	{
		public:
			/**
			 * \brief Default constructor.
			 */
			MoveHistory();

			/**
			 * \brief Creates a \ref MoveHistory object from a list of \ref
			 * Move.
			 * \param src All the previous moves.
			 */
			MoveHistory(const std::vector<Move>& src);

			/**
			 * \brief Adds a new \ref Move to the history.
			 * \param move New \c Move to add.
			 */
			void addNewMove(const Move& move);

			/**
			 * \brief Returns \c true if the piece which starts the game placed
			 * at \a square has ever moved.
			 *
			 * \throws Exception if the square is not occupied by any piece at
			 * the beginning of the game.
			 */
			bool hasEverMoved(const Square& square) const;

			/**
			 * \brief Returns all the moves as a vector.
			 * \return A vector of all moves.
			 */
			const std::vector<Move>& getAllMoves() const;

		private:
			std::vector<Move> mMoves;
			static Board sDefaultBoard;
	};
}

#endif /* MOVE_HISTORY_H_6F6DA4E1_36A3_4F9D_9123_6BA3E98D3D47 */
