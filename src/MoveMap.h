#ifndef MOVE_MAP_H_7CB1008B_77FF_48A5_9C82_0CCC293A4EAA
#define MOVE_MAP_H_7CB1008B_77FF_48A5_9C82_0CCC293A4EAA

#include "movebehaviour/MoveBehaviour.h"

namespace simplechess
{
	/**
	 * Class that maps pieces to their moving behaviours.
	 */
	class MoveMap
	{
		public:
			static const MoveBehaviour& getBehaviourForPiece(PieceType piece);
		private:
			static std::map<PieceType, std::unique_ptr<MoveBehaviour>>
				mPieceToBehaviour;
	};
}

#endif /* MOVE_MAP_H_7CB1008B_77FF_48A5_9C82_0CCC293A4EAA */
