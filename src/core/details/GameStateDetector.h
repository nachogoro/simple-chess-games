#ifndef GAME_STATE_DETECTOR_H_19318747_4966_4AD2_A8A4_173A832713DD
#define GAME_STATE_DETECTOR_H_19318747_4966_4AD2_A8A4_173A832713DD

#include <cpp/simplechess/Game.h>
#include <map>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Relevant information about the state of a game.
		 */
		struct GameStateInformation
		{
			GameStateInformation(
					const GameState gameState,
					const CheckType checkType,
					const std::set<PieceMove>& availableMoves,
					const std::optional<DrawReason>& reasonItWasDrawn,
					const std::optional<DrawReason>& reasonToClaimDraw)
				: gameState(gameState),
				checkType(checkType),
				availableMoves(availableMoves),
				reasonItWasDrawn(reasonItWasDrawn),
				reasonToClaimDraw(reasonToClaimDraw)
			{
			}

			const GameState gameState;
			const CheckType checkType;
			const std::set<PieceMove> availableMoves;
			const std::optional<DrawReason> reasonItWasDrawn;
			const std::optional<DrawReason> reasonToClaimDraw;
		};

		/**
		 * \brief Collection of methods to detect all the relevant information
		 * about the state of a game.
		 */
		class GameStateDetector
		{
			public:
				/**
				 * \brief Returns the information about the state of the game
				 * at a given stage.
				 *
				 * \param stage The current stage of the  game.
				 * \param previouslyReachedPositions A map of how many times
				 * each position has been reached. The positions are described
				 * by their relevant fields in a FEN string (all but the
				 * counters). The map should be the one prior to reaching \a
				 * stage.
				 */
				static GameStateInformation detect(
						const GameStage& stage,
						bool drawOffered,
						const std::map<std::string, uint8_t>& previouslyReachedPositions);
		};
	}
}

#endif
