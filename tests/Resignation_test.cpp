#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(ResignationTest, WhiteResignInTheirTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();
	const Game resigned = gameMgr.resign(startingGame, COLOR_WHITE);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_BLACK_WON);
}

TEST(ResignationTest, WhiteResignInBlackTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = gameMgr.resign(startingGame, COLOR_WHITE);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_BLACK_WON);
}

TEST(ResignationTest, BlackResignInWhiteTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();
	const Game resigned = gameMgr.resign(startingGame, COLOR_BLACK);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_WHITE_WON);
}

TEST(ResignationTest, BlackResignInTheirTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = gameMgr.resign(startingGame, COLOR_BLACK);
	EXPECT_EQ(resigned.gameState(), GAME_STATE_WHITE_WON);
}
