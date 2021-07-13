#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"r1bqkb1r/pppppppp/2n5/8/4n1Q1/2N5/PPPP1PPP/R1B1KBNR w KQkq - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::fromRankAndFile(3, 'c'),
				Square::fromRankAndFile(5, 'b'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Nb5");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureNoCheckNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"r1bqkb1r/pppppppp/2n5/8/2n1P1Q1/2N5/PPP2PPP/R1B1KBNR w KQkq - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::fromRankAndFile(1, 'f'),
				Square::fromRankAndFile(4, 'c'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Bxc4");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"q1q5/q4k2/2P5/3r4/2P1B3/5K2/Q7/8 b - - 1 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_BLACK},
				Square::fromRankAndFile(8, 'c'),
				Square::fromRankAndFile(3, 'h'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Qh3+");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureCheckNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"q7/1P3k2/8/3r4/2P1B2q/5K2/Q7/8 b - - 1 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_BLACK},
				Square::fromRankAndFile(4, 'h'),
				Square::fromRankAndFile(4, 'e'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Qxe4+");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckMateNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"4k3/R6R/8/8/8/8/8/4K3 w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::fromRankAndFile(7, 'h'),
				Square::fromRankAndFile(8, 'h'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Rh8#");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"8/4k3/8/8/8/6K1/8/R6R w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::fromRankAndFile(1, 'h'),
				Square::fromRankAndFile(1, 'd'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Rhd1");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankNoAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"8/4k3/8/8/8/6K1/8/R6R w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::fromRankAndFile(1, 'h'),
				Square::fromRankAndFile(2, 'h'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Rh2");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureNoCheckSameFileAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"b4k2/8/2P5/8/b7/8/8/5K2 b - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_BLACK},
				Square::fromRankAndFile(8, 'a'),
				Square::fromRankAndFile(6, 'c'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "B8xc6");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureCheckSameFileSameRankAmbiguity) {
	const Game game = Game::createGameFromStartingFen(
			"b3bk2/8/2P5/8/b7/5K2/8/8 b - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_BLACK},
				Square::fromRankAndFile(8, 'a'),
				Square::fromRankAndFile(6, 'c'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Ba8xc6+");
}

TEST(AlgebraicNotationTest, PawnPromotionNoCaptureNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(7, 'b'),
				Square::fromRankAndFile(8, 'b'),
				TYPE_QUEEN)).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "b8=Q");
}

TEST(AlgebraicNotationTest, PawnPromotionCaptureCheck) {
	const Game game = Game::createGameFromStartingFen(
			"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(7, 'b'),
				Square::fromRankAndFile(8, 'c'),
				TYPE_ROOK)).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "xc8=R+");
}

TEST(AlgebraicNotationTest, PawnRegularMoveCaptureAmbiguityNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"k7/8/8/3p1p2/4N3/8/8/7K b - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::fromRankAndFile(5, 'd'),
				Square::fromRankAndFile(4, 'e'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "dxe4");
}

TEST(AlgebraicNotationTest, PawnRegularMoveCaptureNoAmbiguityNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"k7/8/8/6pp/7N/8/8/7K b - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::fromRankAndFile(5, 'g'),
				Square::fromRankAndFile(4, 'h'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "xh4");
}

TEST(AlgebraicNotationTest, PawnEnPassantCaptureNoAmbiguityNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"7k/8/8/Pp6/8/7K/8/8 w - b6 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(5, 'a'),
				Square::fromRankAndFile(6, 'b'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "xb6");
}

TEST(AlgebraicNotationTest, PawnEnPassantCaptureAmbiguityNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"7k/8/8/PpP5/8/7K/8/8 w - b6 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(5, 'a'),
				Square::fromRankAndFile(6, 'b'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "axb6");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguityDrawOffer) {
	const Game game = Game::createGameFromStartingFen(
			"8/8/3K4/8/Q7/8/p7/1k6 w - - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::fromRankAndFile(4, 'a'),
				Square::fromRankAndFile(4, 'b')), true).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "Qb4+(=)");
}

TEST(AlgebraicNotationTest, CastlingKingsideNoCheck) {
	const Game game = Game::createGameFromStartingFen(
			"8/8/8/8/6k1/8/4PP1P/4K2R w K - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(1, 'e'),
				Square::fromRankAndFile(1, 'g'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "O-O");
}

TEST(AlgebraicNotationTest, CastlingQueensideCheckmate) {
	const Game game = Game::createGameFromStartingFen(
			"r3k1K1/1q6/8/8/8/8/8/8 b q - 0 1");

	const GameStage stage = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_BLACK},
				Square::fromRankAndFile(8, 'e'),
				Square::fromRankAndFile(8, 'c'))).currentStage();

	EXPECT_EQ(stage.move()->inAlgebraicNotation(), "O-O-O#");
}
