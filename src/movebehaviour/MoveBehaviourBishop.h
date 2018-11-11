#ifndef MOVE_BEHAVIOUR_BISHOP_H_6A8581BD_A99A_4198_92B4_407D85FC0BA5
#define MOVE_BEHAVIOUR_BISHOP_H_6A8581BD_A99A_4198_92B4_407D85FC0BA5

#include "MoveBehaviour.h"

namespace simplechess
{
	class MoveBehaviourBishop : public MoveBehaviour
	{
		public:
			MoveBehaviourBishop();

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
