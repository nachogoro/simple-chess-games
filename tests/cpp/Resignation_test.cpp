#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(ResignationTest, WhiteResignInTheirTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();
	const Game resigned = gameMgr.resign(startingGame, Color::White);
	EXPECT_EQ(resigned.gameState(), GameState::BlackWon);
}

TEST(ResignationTest, WhiteResignInBlackTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = gameMgr.resign(startingGame, Color::White);
	EXPECT_EQ(resigned.gameState(), GameState::BlackWon);
}

TEST(ResignationTest, BlackResignInWhiteTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();
	const Game resigned = gameMgr.resign(startingGame, Color::Black);
	EXPECT_EQ(resigned.gameState(), GameState::WhiteWon);
}

TEST(ResignationTest, BlackResignInTheirTurn) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = gameMgr.resign(startingGame, Color::Black);
	EXPECT_EQ(resigned.gameState(), GameState::WhiteWon);
}
