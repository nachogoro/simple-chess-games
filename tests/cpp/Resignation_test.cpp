#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(ResignationTest, WhiteResignInTheirTurn) {
	const Game startingGame = createNewGame();
	const Game resigned = resign(startingGame, Color::White);
	EXPECT_EQ(resigned.gameState(), GameState::BlackWon);
}

TEST(ResignationTest, WhiteResignInBlackTurn) {
	const Game startingGame = createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = resign(startingGame, Color::White);
	EXPECT_EQ(resigned.gameState(), GameState::BlackWon);
}

TEST(ResignationTest, BlackResignInWhiteTurn) {
	const Game startingGame = createNewGame();
	const Game resigned = resign(startingGame, Color::Black);
	EXPECT_EQ(resigned.gameState(), GameState::WhiteWon);
}

TEST(ResignationTest, BlackResignInTheirTurn) {
	const Game startingGame = createGameFromFen(
			"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
	const Game resigned = resign(startingGame, Color::Black);
	EXPECT_EQ(resigned.gameState(), GameState::WhiteWon);
}
