#ifndef MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F
#define MOVE_VALIDATOR_H_D8B8A755_7C97_4A23_9956_2E4322D4672F

#include <Board.h>
#include <Color.h>
#include <GameStage.h>
#include <PieceMove.h>
#include <Square.h>

#include <boost/optional.hpp>

#include <set>

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
						const PieceMove& pieceMove);

				// Returns all possible moves for activeColor without
				// accounting for whether the move would leave the king in
				// check
				static std::set<PieceMove> allAvailableMovesUnfiltered(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						Color activeColor);

				static std::set<PieceMove> allAvailableMoves(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						Color activeColor);

				static std::set<PieceMove> availableMovesForPieceUnfiltered(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						const Square& square);

				static std::set<PieceMove> availableMovesForPiece(
						const Board& board,
						const boost::optional<Square>& enPassantTarget,
						uint8_t castlingRights,
						const Square& square);
		};
	}
}

#endif
