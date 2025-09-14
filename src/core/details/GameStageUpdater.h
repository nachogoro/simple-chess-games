#ifndef GAME_STAGE_UPDATE_H_098D59B8_936E_4A7B_9A79_F4385132B35F
#define GAME_STAGE_UPDATE_H_098D59B8_936E_4A7B_9A79_F4385132B35F

#include <cpp/simplechess/GameStage.h>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Collection of methods to help update a game stage after a
		 * move.
		 */
		class GameStageUpdater
		{
			public:
				static GameStage makeMove(
						const GameStage& stage,
						const PieceMove& move,
						bool offerDraw);
		};
	}
}

#endif
