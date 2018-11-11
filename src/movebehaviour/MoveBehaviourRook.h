#ifndef MOVE_BEHAVIOUR_ROOK_H_65B90621_5AEC_4C5D_9668_92CD52B89A66
#define MOVE_BEHAVIOUR_ROOK_H_65B90621_5AEC_4C5D_9668_92CD52B89A66

#include "MoveBehaviour.h"

namespace simplechess
{
	class MoveBehaviourRook : public MoveBehaviour
	{
		public:
			MoveBehaviourRook();

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
