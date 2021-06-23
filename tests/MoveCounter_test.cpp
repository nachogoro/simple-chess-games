#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(MoveCounterTest, FullMoveCounterFromStart) {
	const Game game = Game::createNewGame();
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 1);

	const Game afterFirstWhiteMove = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'e'),
				Square::instantiateWithRankAndFile(4, 'e')));

	EXPECT_EQ(afterFirstWhiteMove.currentStage().fullMoveCounter(), 1);

	const Game afterBlackFirstMove = afterFirstWhiteMove.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::instantiateWithRankAndFile(7, 'e'),
				Square::instantiateWithRankAndFile(5, 'e')));

	EXPECT_EQ(afterBlackFirstMove.currentStage().fullMoveCounter(), 2);

	const Game afterWhiteResponse = afterBlackFirstMove.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'g'),
				Square::instantiateWithRankAndFile(3, 'f')));

	EXPECT_EQ(afterWhiteResponse.currentStage().fullMoveCounter(), 2);
}

TEST(MoveCounterTest, FullMoveCounterFromFenStartingWhite) {
	const Game game = Game::createGameFromStartingFen(
			"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 63");
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 63);

	const Game afterWhiteMove = game.makeMove(
			PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(3, 'c'),
				Square::instantiateWithRankAndFile(5, 'c')));

	EXPECT_EQ(afterWhiteMove.currentStage().fullMoveCounter(), 63);

	const Game afterBlackResponse = afterWhiteMove.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::instantiateWithRankAndFile(6, 'g'),
				Square::instantiateWithRankAndFile(5, 'g')));

	EXPECT_EQ(afterBlackResponse.currentStage().fullMoveCounter(), 64);

	const Game afterWhiteNextMove = afterBlackResponse.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'h'),
				Square::instantiateWithRankAndFile(5, 'g')));

	EXPECT_EQ(afterWhiteNextMove.currentStage().fullMoveCounter(), 64);
}

TEST(MoveCounterTest, FullMoveCounterFromFenStartingBlack) {
	const Game game = Game::createGameFromStartingFen(
			"8/4k3/6p1/2n5/7P/2RB4/1K6/8 b - - 0 51");
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 51);

	const Game afterBlackMove = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_BLACK},
				Square::instantiateWithRankAndFile(5, 'c'),
				Square::instantiateWithRankAndFile(3, 'd')));

	EXPECT_EQ(afterBlackMove.currentStage().fullMoveCounter(), 52);

	const Game afterWhiteResponse = afterBlackMove.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'b'),
				Square::instantiateWithRankAndFile(3, 'b')));

	EXPECT_EQ(afterWhiteResponse.currentStage().fullMoveCounter(), 52);

	const Game afterBlackNextMove = afterWhiteResponse.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::instantiateWithRankAndFile(6, 'g'),
				Square::instantiateWithRankAndFile(5, 'g')));

	EXPECT_EQ(afterBlackNextMove.currentStage().fullMoveCounter(), 53);
}

TEST(MoveCounterTest, HalfMoveCounter) {
	const Game game = Game::createNewGame();

	EXPECT_EQ(game.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	// Pawn move does not increase the counter if it is 0
	const Game afterWhite1 = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'e'),
				Square::instantiateWithRankAndFile(4, 'e')));
	EXPECT_EQ(afterWhite1.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	// Non capture or pawn advance moves increase the counter in 1
	const Game afterBlack1 = afterWhite1.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_BLACK},
				Square::instantiateWithRankAndFile(8, 'g'),
				Square::instantiateWithRankAndFile(6, 'f')));
	EXPECT_EQ(afterBlack1.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 1);

	const Game afterWhite2 = afterBlack1.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'b'),
				Square::instantiateWithRankAndFile(3, 'c')));
	EXPECT_EQ(afterWhite2.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 2);

	// Captures reset the counter to 0
	const Game afterBlack2 = afterWhite2.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_BLACK},
				Square::instantiateWithRankAndFile(6, 'f'),
				Square::instantiateWithRankAndFile(4, 'e')));
	EXPECT_EQ(afterBlack2.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	const Game afterWhite3 = afterBlack2.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(4, 'g')));
	EXPECT_EQ(afterWhite3.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 1);

	const Game afterBlack3 = afterWhite3.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_BLACK},
				Square::instantiateWithRankAndFile(8, 'b'),
				Square::instantiateWithRankAndFile(6, 'c')));
	EXPECT_EQ(afterBlack3.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 2);
}
