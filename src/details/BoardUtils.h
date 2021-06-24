#ifndef BOARD_UTILS_H_C5E04339_E38D_43A7_A400_CD4BB2FF38AC
#define BOARD_UTILS_H_C5E04339_E38D_43A7_A400_CD4BB2FF38AC

#include <Board.h>
#include <PossibleMove.h>

namespace simplechess
{
	class BoardUtils
	{
		public:
		static Board createDefaultBoard();
		static Board afterMove(const Board& board, const PossibleMove& move);
	};
}

#endif /* BOARD_UTILS_H_C5E04339_E38D_43A7_A400_CD4BB2FF38AC */
