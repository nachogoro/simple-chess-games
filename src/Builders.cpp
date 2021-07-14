#include "Builders.h"

using namespace simplechess;

GameStage GameStageBuilder::build(
		const Board& board,
		const Color toPlay,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const boost::optional<PlayedMove>& move)
{
	const std::string fen = details::FenUtils::generateFen(
			board,
			toPlay,
			castlingRights,
			halfmoveClock,
			fullmoveClock,);

	return GameStage(
			board,
			toPlay,
			castlingRights,
			halfmoveClock,
			fullmoveClock,
			fen,
			move);
}
