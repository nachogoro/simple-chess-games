#ifndef MOVE_BEHAVIOUR_KING_H_F4A2379C_D3BD_422A_B39B_BB32F1054E31
#define MOVE_BEHAVIOUR_KING_H_F4A2379C_D3BD_422A_B39B_BB32F1054E31

#include "MoveBehaviour.h"

namespace simplechess
{
	class MoveBehaviourKing : public MoveBehaviour
	{
		public:
			virtual std::vector<PossibleMove> possibleMoves(
					const Square& srcSquare,
					const Board& board,
					const MoveHistory& moveHistory) const;

			virtual bool isValidMove(
					const Move& move,
					const Board& board,
					const MoveHistory& moveHistory) const;

			virtual std::unique_ptr<MoveBehaviour> clone() const;
	};
}

#endif
