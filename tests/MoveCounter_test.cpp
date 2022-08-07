#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(MoveCounterTest, FullMoveCounterFromStart) {
	const GameManager gameMgr;
	const Game game = gameMgr.createNewGame();
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 1);

	const Game afterFirstWhiteMove = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(2, 'e'),
				Square::fromRankAndFile(4, 'e')));

	EXPECT_EQ(afterFirstWhiteMove.currentStage().fullMoveCounter(), 1);

	const Game afterBlackFirstMove = gameMgr.makeMove(
			afterFirstWhiteMove,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromRankAndFile(7, 'e'),
				Square::fromRankAndFile(5, 'e')));

	EXPECT_EQ(afterBlackFirstMove.currentStage().fullMoveCounter(), 2);

	const Game afterWhiteResponse = gameMgr.makeMove(
			afterBlackFirstMove,
			PieceMove::regularMove(
				{PieceType::Knight, Color::White},
				Square::fromRankAndFile(1, 'g'),
				Square::fromRankAndFile(3, 'f')));

	EXPECT_EQ(afterWhiteResponse.currentStage().fullMoveCounter(), 2);
}

TEST(MoveCounterTest, FullMoveCounterFromFenStartingWhite) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 63");
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 63);

	const Game afterWhiteMove = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Rook, Color::White},
				Square::fromRankAndFile(3, 'c'),
				Square::fromRankAndFile(5, 'c')));

	EXPECT_EQ(afterWhiteMove.currentStage().fullMoveCounter(), 63);

	const Game afterBlackResponse = gameMgr.makeMove(
			afterWhiteMove,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromRankAndFile(6, 'g'),
				Square::fromRankAndFile(5, 'g')));

	EXPECT_EQ(afterBlackResponse.currentStage().fullMoveCounter(), 64);

	const Game afterWhiteNextMove = gameMgr.makeMove(
			afterBlackResponse,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(4, 'h'),
				Square::fromRankAndFile(5, 'g')));

	EXPECT_EQ(afterWhiteNextMove.currentStage().fullMoveCounter(), 64);
}

TEST(MoveCounterTest, FullMoveCounterFromFenStartingBlack) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/4k3/6p1/2n5/7P/2RB4/1K6/8 b - - 0 51");
	EXPECT_EQ(game.currentStage().fullMoveCounter(), 51);

	const Game afterBlackMove = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Knight, Color::Black},
				Square::fromRankAndFile(5, 'c'),
				Square::fromRankAndFile(3, 'd')));

	EXPECT_EQ(afterBlackMove.currentStage().fullMoveCounter(), 52);

	const Game afterWhiteResponse = gameMgr.makeMove(
			afterBlackMove,
			PieceMove::regularMove(
				{PieceType::King, Color::White},
				Square::fromRankAndFile(2, 'b'),
				Square::fromRankAndFile(3, 'b')));

	EXPECT_EQ(afterWhiteResponse.currentStage().fullMoveCounter(), 52);

	const Game afterBlackNextMove = gameMgr.makeMove(
			afterWhiteResponse,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromRankAndFile(6, 'g'),
				Square::fromRankAndFile(5, 'g')));

	EXPECT_EQ(afterBlackNextMove.currentStage().fullMoveCounter(), 53);
}

TEST(MoveCounterTest, HalfMoveCounter) {
	const GameManager gameMgr;
	const Game game = gameMgr.createNewGame();

	EXPECT_EQ(game.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	// Pawn move does not increase the counter if it is 0
	const Game afterWhite1 = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(2, 'e'),
				Square::fromRankAndFile(4, 'e')));
	EXPECT_EQ(afterWhite1.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	// Non capture or pawn advance moves increase the counter in 1
	const Game afterBlack1 = gameMgr.makeMove(
			afterWhite1,
			PieceMove::regularMove(
				{PieceType::Knight, Color::Black},
				Square::fromRankAndFile(8, 'g'),
				Square::fromRankAndFile(6, 'f')));
	EXPECT_EQ(afterBlack1.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 1);

	const Game afterWhite2 = gameMgr.makeMove(
			afterBlack1,
			PieceMove::regularMove(
				{PieceType::Knight, Color::White},
				Square::fromRankAndFile(1, 'b'),
				Square::fromRankAndFile(3, 'c')));
	EXPECT_EQ(afterWhite2.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 2);

	// Captures reset the counter to 0
	const Game afterBlack2 = gameMgr.makeMove(
			afterWhite2,
			PieceMove::regularMove(
				{PieceType::Knight, Color::Black},
				Square::fromRankAndFile(6, 'f'),
				Square::fromRankAndFile(4, 'e')));
	EXPECT_EQ(afterBlack2.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 0);

	const Game afterWhite3 = gameMgr.makeMove(
			afterBlack2,
			PieceMove::regularMove(
				{PieceType::Queen, Color::White},
				Square::fromRankAndFile(1, 'd'),
				Square::fromRankAndFile(4, 'g')));
	EXPECT_EQ(afterWhite3.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 1);

	const Game afterBlack3 = gameMgr.makeMove(
			afterWhite3,
			PieceMove::regularMove(
				{PieceType::Knight, Color::Black},
				Square::fromRankAndFile(8, 'b'),
				Square::fromRankAndFile(6, 'c')));
	EXPECT_EQ(afterBlack3.currentStage().halfMovesSinceLastCaptureOrPawnAdvance(), 2);
}
