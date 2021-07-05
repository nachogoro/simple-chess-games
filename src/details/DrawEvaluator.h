#ifndef DRAW_EVALUATOR_H_B4AF96CF_8DE1_4557_96AD_BF906AE79F49
#define DRAW_EVALUATOR_H_B4AF96CF_8DE1_4557_96AD_BF906AE79F49

#include <Game.h>
#include <GameStage.h>

#include <boost/optional.hpp>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Collection of static methods to evaluate and infer the state
		 * of a \ref Game as it relates to drawing.
		 */
		class DrawEvaluator
		{
			public:
				/**
				 * \brief Returns a reason why a game at this stage could be
				 * drawn.
				 *
				 * As it has no information about previous reached positions,
				 * it cannot make claims about n-fold repetition.
				 *
				 * \note If more than one reason exists and at least one of
				 * them is non-optional (i.e. stalemate, 75-move rule,
				 * insufficient material), the non-optional one will be
				 * returned.
				 *
				 * \param stage The stage of the game being evaluated.
				 *
				 * \return A reason why the game could be drawn, or an empty
				 * value if no such reason is found.
				 */
				static boost::optional<DrawReason> reasonToDraw(
						const GameStage& stage);

				/**
				 * \brief Returns a reason why a game at this stage could be
				 * drawn.
				 *
				 * \note If more than one reason exists and at least one of
				 * them is non-optional (i.e. stalemate, 5-fold repetition,
				 * 75-move rule, insufficient material), the non-optional one
				 * will be returned.
				 *
				 * \param stage The stage of the game being evaluated.
				 * \param previouslyReachedPositions Map of positions
				 * previously reached in the game and the number of times they
				 * were reached, necessary to evaluate n-fold repetition.
				 *
				 * \return A reason why the game could be drawn, or an empty
				 * value if no such reason is found.
				 */
				static boost::optional<DrawReason> reasonToDraw(
						const GameStage& stage,
						std::map<std::string, uint8_t> previouslyReachedPositions);
		};
	}
}

#endif
