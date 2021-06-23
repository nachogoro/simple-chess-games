#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(ResignationTest, WhiteResignInTheirTurn) {
	const Game startingGame = Game::createNewGame();
	const Game resigned = startingGame.resign(COLOR_WHITE);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_BLACK_WON);
}

TEST(ResignationTest, WhiteResignInBlackTurn) {
	const Game startingGame = Game::createGameFromStartingFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = startingGame.resign(COLOR_WHITE);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_BLACK_WON);
}

TEST(ResignationTest, BlackResignInWhiteTurn) {
	const Game startingGame = Game::createNewGame();
	const Game resigned = startingGame.resign(COLOR_BLACK);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_WHITE_WON);
}

TEST(ResignationTest, BlackResignInTheirTurn) {
	const Game startingGame = Game::createGameFromStartingFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = startingGame.resign(COLOR_BLACK);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_WHITE_WON);
}
