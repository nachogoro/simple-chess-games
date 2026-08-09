#ifndef GAME_STAGE_UPDATE_H_098D59B8_936E_4A7B_9A79_F4385132B35F
#define GAME_STAGE_UPDATE_H_098D59B8_936E_4A7B_9A79_F4385132B35F

#include <cpp/simplechess/GameStage.h>

#include "MoveValidator.h"

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief A new stage together with everything which had to be derived
		 * from its board in order to build it.
		 *
		 * The analysis is handed back rather than discarded so that callers
		 * which need the legal moves of the new position (such as the game
		 * state detector) do not have to generate them a second time.
		 */
		struct UpdatedStage
		{
			GameStage stage;
			PositionAnalysis analysis;
		};

		/**
		 * \brief Collection of methods to help update a game stage after a
		 * move.
		 */
		class GameStageUpdater
		{
			public:
				/**
				 * \brief Returns the stage resulting from playing \p move.
				 *
				 * The check status of the resulting stage is derived on
				 * demand; prefer \ref makeMoveWithAnalysis when the legal
				 * moves of the resulting position are also needed.
				 */
				static GameStage makeMove(
						const GameStage& stage,
						const PieceMove& move);

				/**
				 * \brief Returns the stage resulting from playing \p move,
				 * along with the analysis of the resulting position.
				 */
				static UpdatedStage makeMoveWithAnalysis(
						const GameStage& stage,
						const PieceMove& move);
		};
	}
}

#endif
