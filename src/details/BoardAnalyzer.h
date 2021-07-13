#ifndef BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36
#define BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36

#include <Board.h>
#include <Color.h>
#include <Square.h>

#include <boost/optional.hpp>

#include <set>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Collection of tools to analyze a board.
		 */
		class BoardAnalyzer
		{
			public:
				/**
				 * \brief Whether if \a square in \a board is threatened by any
				 * piece of color \a color.
				 *
				 * A square is considered threatened if a piece of the
				 * specified \a color could occupy it in a single move if the
				 * square were to be occupied by a piece of the color opposite
				 * to \a color.
				 *
				 * \param board The board to be inspected.
				 * \param square The square being queried.
				 * \param color The "attacking" color.
				 * \return \c true if \a square in \a board is threatened by
				 * any piece of color \a color, \c false otherwise.
				 */
				static bool isSquareThreatenedBy(
						const Board& board,
						const Square& square,
						Color color);

				/**
				 * \brief Whether the King of color \a color is in check.
				 *
				 * \param board The board to be inspected.
				 * \param color The color of the King.
				 * \return \c true if the King is in check, \c false otherwise.
				 */
				static bool isInCheck(
						const Board& board,
						Color color);

				/**
				 * \brief Whether \a square is not occupied by any piece.
				 * \param board The state of the board.
				 * \param square Square being queried.
				 * \return \c true if \a square is not occupied by any piece,
				 * \c false otherwise.
				 */
				static bool isEmpty(const Board& board, const Square& square);

				/**
				 * \brief Whether a piece of color \a color could theoretically
				 * move into \a dstSquare.
				 *
				 * This method does not check if there are any actual pieces of
				 * said \a color in a position to occupy the \a dstSquare, only
				 * if a piece of said color could theoretically land on that
				 * square (i.e. the square is either empty or occupied by a
				 * piece of the opposite color).
				 *
				 * \note If a piece of color \a color already occupies the \a
				 * dstSquare, this method returns \c false.
				 *
				 * \param board The state of the board.
				 * \param dstSquare Square being queried.
				 * \param color The color of the piece which would move to the
				 * \a dstSquare.
				 * \return \c true if the \a dstSquare is empty or occupied by
				 * a piece of the opposite color, \c false otherwise.
				 */
				static bool isOccupiableBy(
						const Board& board,
						const Square& dstSquare,
						Color color);

				/**
				 * \brief Whether \a square is occupied by a piece of color \a
				 * color.
				 * \param board The state of the board.
				 * \param square Square being queried.
				 * \param color The color of the piece which would move to the
				 * \a dstSquare.
				 * \return \c true if the \a square is occupied by a piece of
				 * color \a color, \c false otherwise.
				 */
				static bool isOccupiedByPieceOfColor(
						const Board& board, const Square& square, Color color);

				/**
				 * \brief Returns all the squares which could be reached by a
				 * piece of color \a color from \a src by moving in the
				 * direction specified by \a rankStep and \a fileStep.
				 *
				 * All squares in a given direction from \a src are reachable
				 * until:
				 * - A square is reached which cannot be occupied by a piece of
				 *   color \a color.
				 * - The first square occupied by a piece of the opposite color
				 *   is reached.
				 * - The end of the board is reached.
				 *
				 * \param board The state of the board.
				 * \param src The square from where the piece would start.
				 * \param color The color of the moving piece.
				 * \param rankStep How much would the rank increments in each
				 * move.
				 * \param fileStep How much would the file increments in each
				 * move.
				 * \return \c true if they are in the same rank or file, \c
				 * false otherwise.
				 */
				static std::set<Square> reachableSquaresInDirection(
						const Board& board,
						const Square& src,
						Color color,
						int8_t rankStep,
						int8_t fileStep);

				/**
				 * \brief Returns the \ref Square occupied by the King of the
				 * specified \a color.
				 * \param board The state of the board.
				 * \param color The color of the King being queried.
				 * \return The \c Square occupied by the King of the specified
				 * \a color.
				 */
				static const Square& kingSquare(const Board& board, Color color);

				/**
				 * \brief Returns a new copy of the state of the board after
				 * making the \p move.
				 *
				 * \note This method does not validate whether the move is
				 * legal according to the state of the board or how pieces
				 * move.
				 *
				 * \param board The state of the board on which the move is to
				 * be made.
				 * \param move Move to be made.
				 * \return A new copy of the state of the board after the move.
				 */
				static Board makeMoveOnBoard(
						const Board& board,
						const PieceMove& move);
		};
	}
}

#endif /* BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36 */
