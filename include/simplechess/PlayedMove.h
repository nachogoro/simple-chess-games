#ifndef MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC
#define MOVE_H_BE1088E7_A628_4046_BB4C_41A12DAA23BC

#include <simplechess/Board.h>
#include <simplechess/Piece.h>
#include <simplechess/PieceMove.h>
#include <simplechess/Square.h>

#include <optional>

#include <string>

namespace simplechess
{
	class PlayedMoveBuilder;

	/**
	 * The different types of check which can be caused by a move.
	 */
	enum class CheckType
	{
		/**
		 * No check whatsoever.
		 */
		NoCheck,

		/**
		 * Regular check (the other side still has valid moves to break the
		 * check).
		 */
		Check,

		/**
		 * Checkmate (the other side has no valid moves).
		 */
		CheckMate
	};

	/**
	 * \brief Describes a move that has been played in a game of chess.
	 *
	 * A played move is defined both by the "abstract" piece movement
	 * information (e.g. "The bishop on b5 moved to c6") as well as context
	 * of the situation when it was played (e.g. "The bishop captured a rook",
	 * "The move included a draw offer", "The move resulted in checkmate",
	 * etc.).
	 */
	class PlayedMove
	{
		public:

			/**
			 * \brief Returns the string representation of the move in
			 * algebraic notation.
			 * \return The string representation of the move in algebraic
			 * notation.
			 */
			const std::string& inAlgebraicNotation() const;

			/**
			 * \brief Returns the description of the move of the piece as a
			 * \ref PieceMove.
			 * \return The description of the move of the piece.
			 */
			const PieceMove& pieceMove() const;

			/**
			 * \brief Returns the opponent's captured piece in this move, if
			 * any, or an empty optional otherwise.
			 *
			 * \return \c true if an opponent's piece has been captured in this
			 * move, \c false otherwise.
			 */
			const std::optional<Piece>& capturedPiece() const;

			/**
			 * \brief The type of check delivered by the move.
			 *
			 * \return The type of check delivered by the move.
			 */
			CheckType checkType() const;

			/**
			 * \brief Whether the player offers a draw with this move.
			 *
			 * If a draw is offered, the oponent is allowed to make an
			 * ACCEPT_DRAW move in the immediately next turn.
			 *
			 * \return \c true if a draw is being offered, \c false otherwise.
			 */
			bool isDrawOffered() const;

		private:
			PlayedMove(
					const PieceMove& pieceMove,
					const std::optional<Piece>& capturedPiece,
					bool drawOffered,
					CheckType checkType,
					const std::string& algebraicNotation);

			friend class PlayedMoveBuilder;

			PieceMove mPieceMove;
			std::optional<Piece> mCapturedPiece;
			bool mDrawOffered;
			CheckType mCheckType;
			std::string mAlgebraicNotation;
	};
}

#endif
