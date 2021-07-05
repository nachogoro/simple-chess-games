#ifndef BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36
#define BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36

#include <Board.h>
#include <Color.h>

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
				 * specified \a color could occupy it in a single move.
				 *
				 * \note In the case of en passant capture, the threatened
				 * square is the one "behind" the pawn, as it is the one
				 * susceptible of being occupied.
				 *
				 * \param board The board to be inspected.
				 * \param enPassantTarget The optional \c Square which is
				 * a target of en passant capture (the one "behind" the pawn).
				 * \param castlingRights A bit mask of the types of \ref
				 * CastlingRight available in the position.
				 * \param square The square being queried.
				 * \param color The "attacking" color.
				 * \return \c true if \a square in \a board is threatened by
				 * any piece of color \a color, \c false otherwise.
				 */
				static bool isSquareThreatenedBy(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						const Square& square,
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
				 * \brief Whether two squares are connected diagonally.
				 * \param src One of the squares.
				 * \param dst The other square.
				 * \return \c true if they are connected diagonally, \c false
				 * otherwise.
				 */
				static bool isInDiagonal(const Square& src, const Square& dst);

				/**
				 * \brief Whether two squares are in the same rank or file.
				 * \param src One of the squares.
				 * \param dst The other square.
				 * \return \c true if they are in the same rank or file, \c
				 * false otherwise.
				 */
				static bool isInSameRankOrFile(const Square& src, const Square& dst);

				/**
				 * \brief Whether there is a clear line of sight between two
				 * squares, either diagonally, vertically or horizontally.
				 *
				 * A clear line of sight means that both squares are in the
				 * same file, rank or diagonal line and the squares between
				 * them are empty (excluding \a src and \dst).
				 *
				 * \param board The state of the board.
				 * \param src One of the squares.
				 * \param dst The other square.
				 * \return \c true if they are in the same rank or file, \c
				 * false otherwise.
				 */
				static bool isReachable(
						const Board& board, const Square& src, const Square& dst);

				/**
				 * \brief Returns the \ref Square occupied by the King of the
				 * specified \a color.
				 * \param board The state of the board.
				 * \param color The color of the King being queried.
				 * \return The \c Square occupied by the King of the specified
				 * \a color.
				 */
				static const Square& kingSquare(const Board& board, Color color);
		};
	}
}

#endif /* BOARD_ANALYZER_H_9FC5BA38_BA75_4088_AB11_9AF9248ACD36 */
