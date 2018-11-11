#ifndef MOVE_BEHAVIOUR_KNIGHT_H_96D0FD5E_12E7_45D9_9CBE_002D8F91DB94
#define MOVE_BEHAVIOUR_KNIGHT_H_96D0FD5E_12E7_45D9_9CBE_002D8F91DB94

#include "MoveBehaviour.h"

namespace simplechess
{
	class MoveBehaviourKnight : public MoveBehaviour
	{
		public:
			MoveBehaviourKnight();

			virtual std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory) const;

			virtual bool isValidMove(
					const Move& move,
					const BoardImpl& board,
					const std::vector<Move>& moveHistory) const;

			virtual std::unique_ptr<MoveBehaviour> clone() const;
	};
}

#endif
