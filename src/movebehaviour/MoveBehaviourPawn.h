#ifndef MOVE_BEHAVIOUR_PAWN_H_B0EC2F0B_327C_4CBB_B56E_DC0187436F4C
#define MOVE_BEHAVIOUR_PAWN_H_B0EC2F0B_327C_4CBB_B56E_DC0187436F4C

#include "MoveBehaviour.h"

namespace simplechess
{
	class MoveBehaviourPawn : public MoveBehaviour
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
