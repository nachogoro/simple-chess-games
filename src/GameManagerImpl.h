#ifndef GAME_MANAGER_IMPL_H_B6E57879_AFBD_4228_A534_0CA2FA91E149
#define GAME_MANAGER_IMPL_H_B6E57879_AFBD_4228_A534_0CA2FA91E149

#include <simplechess/Color.h>
#include <simplechess/Game.h>
#include <simplechess/PieceMove.h>

namespace simplechess
{
	class GameManagerImpl
	{
		public:
			Game createNewGame() const;

			Game createGameFromFen(const std::string& fen) const;

			Game makeMove(
					const Game& game,
					const PieceMove& move,
					bool offerDraw) const;

			Game claimDraw(const Game& game) const;

			Game resign(const Game& game, Color resigningPlayer) const;
	};
}

#endif
