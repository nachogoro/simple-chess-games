#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(GameCreationTest, RegularGameCreation) {
	const Game game = Game::createNewGame();

	EXPECT_EQ(game.gameState(), GAME_STATE_PLAYING);

	EXPECT_THROW_CUSTOM(game.drawReason(), IllegalStateException);

	const std::vector<GameStage>& history = game.gameHistory();

	EXPECT_EQ(history.size(), 1);
	EXPECT_EQ(game.activeColor(), COLOR_WHITE);
	EXPECT_EQ(game.reasonToClaimDraw(), boost::optional<DrawReason>());

	// Validate piece positions
	const std::map<Square, Piece> expectedPositions = {
		{Square::instantiateWithRankAndFile(8, 'a'), {TYPE_ROOK, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'b'), {TYPE_KNIGHT, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'c'), {TYPE_BISHOP, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'd'), {TYPE_QUEEN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'e'), {TYPE_KING, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'f'), {TYPE_BISHOP, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'g'), {TYPE_KNIGHT, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'h'), {TYPE_ROOK, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'a'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'b'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'c'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'd'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'e'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'f'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'g'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'h'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(1, 'a'), {TYPE_ROOK, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'b'), {TYPE_KNIGHT, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'c'), {TYPE_BISHOP, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'd'), {TYPE_QUEEN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'e'), {TYPE_KING, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'f'), {TYPE_BISHOP, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'g'), {TYPE_KNIGHT, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(1, 'h'), {TYPE_ROOK, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'a'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'b'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'c'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'd'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'e'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'f'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'g'), {TYPE_PAWN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(2, 'h'), {TYPE_PAWN, COLOR_WHITE}},
	};

	EXPECT_EQ(history[history.size() - 1].board().occupiedSquares(), expectedPositions);
}

TEST(GameCreationTest, GameCreationFromPosition1) {
	const Game game = Game::createGameFromStartingFen(
			"5rk1/3Q1p1p/6p1/8/3B4/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GAME_STATE_PLAYING);

	EXPECT_THROW_CUSTOM(game.drawReason(), IllegalStateException);

	const std::vector<GameStage>& history = game.gameHistory();

	EXPECT_EQ(history.size(), 1);
	EXPECT_EQ(game.activeColor(), COLOR_BLACK);
	EXPECT_EQ(game.reasonToClaimDraw(), boost::optional<DrawReason>());

	// Validate piece positions
	const std::map<Square, Piece> expectedPositions = {
		{Square::instantiateWithRankAndFile(8, 'f'), {TYPE_ROOK, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(8, 'g'), {TYPE_KING, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'd'), {TYPE_QUEEN, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(7, 'f'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(7, 'h'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(6, 'g'), {TYPE_PAWN, COLOR_BLACK}},
		{Square::instantiateWithRankAndFile(4, 'd'), {TYPE_BISHOP, COLOR_WHITE}},
		{Square::instantiateWithRankAndFile(3, 'e'), {TYPE_KING, COLOR_WHITE}},
	};

	EXPECT_EQ(history[history.size() - 1].board().occupiedSquares(), expectedPositions);

}

TEST(GameCreationTest, GameCreationFromPositionInCheckmate) {
	const Game game = Game::createGameFromStartingFen(
			"6kr/5Q1p/3N2p1/8/8/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GAME_STATE_WHITE_WON);
}

TEST(GameCreationTest, GameCreationFromPositionInStalemate) {
	const Game game = Game::createGameFromStartingFen(
			"7k/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(game.drawReason(), DRAW_REASON_STALEMATE);
}

TEST(GameCreationTest, GameCreationWithTooManyKings) {
	EXPECT_THROW_CUSTOM(
			Game::createGameFromStartingFen("5kk1/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1"),
			std::invalid_argument);
}

TEST(GameCreationTest, GameCreationWithTooFewKings) {
	EXPECT_THROW_CUSTOM(
			Game::createGameFromStartingFen("8/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1"),
			std::invalid_argument);
}

TEST(GameCreationTest, GameCreationActiveSideAlreadyChecking) {
	EXPECT_THROW_CUSTOM(
			Game::createGameFromStartingFen("k4n2/5n1K/8/8/8/8/8/6r1 b - - 0 1"),
			std::invalid_argument);
}
