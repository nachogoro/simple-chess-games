#ifndef MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F
#define MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F

#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/Color.h>
#include <cpp/simplechess/GameStage.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/Square.h>

#include <optional>

#include <set>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Everything which has to be derived from a position by
		 * examining the board, gathered so that it is only computed once.
		 *
		 * Determining the check status of a position already requires
		 * generating the legal moves (to tell check from checkmate), and
		 * evaluating the state of the game requires them again. Passing this
		 * struct around avoids repeating that work.
		 */
		struct PositionAnalysis
		{
			CheckType checkType;
			std::set<PieceMove> legalMoves;

			bool inCheck() const
			{
				return checkType != CheckType::NoCheck;
			}
		};

		/**
		 * \brief Derives the check status and the legal moves of a position
		 * in a single pass.
		 */
		PositionAnalysis analyzePosition(
				const Board& board,
				Color activeColor,
				const std::optional<Square>& enPassantTarget,
				uint8_t castlingRights);

		/**
		 * \brief Returns the castling rights which remain after \p move is
		 * played in a position which currently holds \p castlingRights.
		 */
		uint8_t updatedCastlingRights(
				uint8_t castlingRights,
				const PieceMove& move);

		/**
		 * \brief Collection of methods to validate moves on a board.
		 */
		class MoveValidator
		{
			public:
				static std::optional<Square> enPassantTarget(
						const Board& board,
						const PieceMove& pieceMove);

				static std::set<PieceMove> allAvailableMoves(
						const Board& board,
						const std::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						Color activeColor);

				static std::set<PieceMove> availableMovesForPiece(
						const Board& board,
						const std::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						const Square& square);
		};
	}
}

#endif
