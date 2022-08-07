#ifndef BUILDERS_H_E5875D9E_C927_464E_BC23_2AA288A34B41
#define BUILDERS_H_E5875D9E_C927_464E_BC23_2AA288A34B41

#include <simplechess/Game.h>
#include <simplechess/GameStage.h>

/**
 * This file contains all the builders to create objects from the public
 * interface which are not supposed to be created by the outside user of the
 * library.
 *
 * The purpose of this scheme is to keep the constructors private in those
 * objects while at the same time not having to list every single internal
 * class which might instantiate them in the public header files.
 */

namespace simplechess
{
	class GameStageBuilder
	{
		public:
			static GameStage build(
				const Board& board,
				Color toPlay,
				uint8_t castlingRights,
				uint16_t halfmoveClock,
				uint16_t fullmoveClock,
				const std::optional<PlayedMove>& move);
	};

	class GameBuilder
	{
		public:
			static Game build(
					GameState gameState,
					const std::optional<DrawReason>& drawReason,
					const std::vector<GameStage>& history,
					const std::set<PieceMove>& allAvailableMoves,
					const std::optional<DrawReason>& reasonToClaimDraw);
	};

	class BoardBuilder
	{
		public:
			static Board build(
					const std::map<Square, Piece> positions);
	};

	class PlayedMoveBuilder
	{
		public:
			static PlayedMove build(
					const Board& board,
					const PieceMove& move,
					const bool drawOffered);
	};
}

#endif
