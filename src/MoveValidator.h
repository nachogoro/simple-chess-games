#ifndef MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F
#define MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F

#include <GameStage.h>
#include <PieceMove.h>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Collection of methods to validate moves on a board.
		 */
		class MoveValidator
		{
			public:
				static boost::optional<Square> enPassantTarget(
						const GameStage& stage)
				{
				}

				static std::set<PieceMove> allValidMoves(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						Color activeColor);

				static std::set<PieceMove> availableMovesForPiece(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						const Square& square);
		};
	}
}

#endif
