#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(FenGenerationTest, BlackMoveNoCapture) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR b KQkq - 3 5");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_BLACK},
				Square::fromRankAndFile(8, 'd'),
				Square::fromRankAndFile(6, 'f')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnb1kbnr/ppp2ppp/5q2/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR w KQkq - 4 6");
}

TEST(FenGenerationTest, BlackMoveWithCapture) {
	const Game game = Game::createGameFromStartingFen(
			"8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 b - - 15 26");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_BLACK},
				Square::fromRankAndFile(4, 'h'),
				Square::fromRankAndFile(6, 'h')));

	EXPECT_EQ(result.currentStage().fen(),
			"8/pB1K4/7q/8/1RnP1P2/4P3/4k3/8 w - - 0 27");
}

TEST(FenGenerationTest, BlackPawnMove) {
	const Game game = Game::createGameFromStartingFen(
			"8/1B1K4/7N/8/1RnP1P1q/4P3/p3k3/8 b - - 15 26");

	const Game result = game.makeMove(
			PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_BLACK},
				Square::fromRankAndFile(2, 'a'),
				Square::fromRankAndFile(1, 'a'),
				TYPE_QUEEN));

	EXPECT_EQ(result.currentStage().fen(),
			"8/1B1K4/7N/8/1RnP1P1q/4P3/4k3/q7 w - - 0 27");
}

TEST(FenGenerationTest, WhiteMoveNoCapture) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR w KQkq - 3 5");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::fromRankAndFile(1, 'b'),
				Square::fromRankAndFile(3, 'c')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/2NP4/PPP2PPP/R2QKBNR b KQkq - 4 5");
}

TEST(FenGenerationTest, WhiteMoveWithCapture) {
	const Game game = Game::createGameFromStartingFen(
			"8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 w - - 1 30");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::fromRankAndFile(4, 'b'),
				Square::fromRankAndFile(4, 'c')));

	EXPECT_EQ(result.currentStage().fen(),
			"8/pB1K4/7N/8/2RP1P1q/4P3/4k3/8 b - - 0 30");
}

TEST(FenGenerationTest, WhitePawnMove) {
	const Game game = Game::createGameFromStartingFen(
			"8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 w - - 12 29");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(4, 'f'),
				Square::fromRankAndFile(5, 'f')));

	EXPECT_EQ(result.currentStage().fen(),
			"8/pB1K4/7N/5P2/1RnP3q/4P3/4k3/8 b - - 0 29");
}

TEST(FenGenerationTest, EnPassantTaken) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/pppp1ppp/8/8/3Pp3/2N5/PPP1PPPP/R1BQKBNR b KQkq d3 0 1");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::fromRankAndFile(4, 'e'),
				Square::fromRankAndFile(3, 'd')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnbqkbnr/pppp1ppp/8/8/8/2Np4/PPP1PPPP/R1BQKBNR w KQkq - 0 2");
}

TEST(FenGenerationTest, EnPassantIgnored) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/pppp1ppp/8/8/3Pp3/2N5/PPP1PPPP/R1BQKBNR b KQkq d3 0 1");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::fromRankAndFile(4, 'e'),
				Square::fromRankAndFile(3, 'e')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnbqkbnr/pppp1ppp/8/8/3P4/2N1p3/PPP1PPPP/R1BQKBNR w KQkq - 0 2");
}

TEST(FenGenerationTest, EnPassantIsCreated) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPP1PPP/RNBQKBNR w KQkq e6 0 2");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::fromRankAndFile(2, 'f'),
				Square::fromRankAndFile(4, 'f')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnbqkbnr/pppp1ppp/8/4p3/5P2/4P3/PPPP2PP/RNBQKBNR b KQkq f3 0 2");
}

TEST(FenGenerationTest, WhiteKingsideCastling) {
	const Game game = Game::createGameFromStartingFen(
			"rn1qkbnr/pp2pppp/2p5/1B1p4/4P1b1/5N2/PPPP1PPP/RNBQK2R w KQkq - 1 6");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(1, 'e'),
				Square::fromRankAndFile(1, 'g')));

	EXPECT_EQ(result.currentStage().fen(),
			"rn1qkbnr/pp2pppp/2p5/1B1p4/4P1b1/5N2/PPPP1PPP/RNBQ1RK1 b kq - 2 6");
}

TEST(FenGenerationTest, WhiteQueensideCastling) {
	const Game game = Game::createGameFromStartingFen(
			"rnbq1rk1/ppp2ppp/5n2/3pp3/1b2P3/2NP1Q2/PPPB1PPP/R3KBNR w KQ - 9 10");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(1, 'e'),
				Square::fromRankAndFile(1, 'c')));

	EXPECT_EQ(result.currentStage().fen(),
			"rnbq1rk1/ppp2ppp/5n2/3pp3/1b2P3/2NP1Q2/PPPB1PPP/2KR1BNR b - - 10 10");
}

TEST(FenGenerationTest, BlackKingsideCastling) {
	const Game game = Game::createGameFromStartingFen(
			"3rk2r/8/8/8/8/8/8/3RK2R b Kk - 0 41");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_BLACK},
				Square::fromRankAndFile(8, 'e'),
				Square::fromRankAndFile(8, 'g')));

	EXPECT_EQ(result.currentStage().fen(),
			"3r1rk1/8/8/8/8/8/8/3RK2R w K - 1 42");
}

TEST(FenGenerationTest, BlackQueensideCastling) {
	const Game game = Game::createGameFromStartingFen(
			"r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 7 52");

	const Game result = game.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_BLACK},
				Square::fromRankAndFile(8, 'e'),
				Square::fromRankAndFile(8, 'c')));

	EXPECT_EQ(result.currentStage().fen(),
			"2kr3r/8/8/8/8/8/8/R3K2R w KQ - 8 53");
}
