#ifndef BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36
#define BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36

#include "MoveHistory.h"
#include <Board.h>

namespace simplechess
{
	/**
	 * \brief Collection of tools to analyze a board.
	 */
	class BoardAnalyzer
	{
		public:
		/**
		 * \brief Whether if \a square in \a board is threatened by any piece
		 * of color \a color.
		 * \return \c true if \a square in \a board is threatened by any piece
		 * of color \a color, \c false otherwise.
		 */
		static bool isSquareThreatenedBy(
				const Board& board,
				const Square& square,
				Color color,
				const MoveHistory& moveHistory);

		/**
		 * Whether square is not occupied by any piece.
		 */
		static bool isEmpty(const Board& board, const Square& square);

		/**
		 * Whether a piece of color color could move into dstSquare.
		 */
		static bool isOccupiableBy(
				const Board& board, const Square& dstSquare, Color color);

		/**
		 * Whether dstSquare is occupied by a piece of color color.
		 */
		static bool isOccupiedByPieceOfColor(
				const Board& board, const Square& dstSquare, Color color);

		/**
		 * Whether both squares are in the same diagonal.
		 */
		static bool isInDiagonal(const Square& src, const Square& dst);

		/**
		 * Whether both squares share rank or file.
		 */
		static bool isInSameRankOrFile(const Square& src, const Square& dst);

		/**
		 * Whether there is clean line of sight between src and dst,
		 * whether horizontal, vertical or diagonal.
		 */
		static bool isReachable(
				const Board& board, const Square& src, const Square& dst);

		/**
		 * Returns the position of the king of the specified color.
		 */
		static const Square& kingSquare(const Board& board, Color color);
	};
}

#endif /* BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36 */
