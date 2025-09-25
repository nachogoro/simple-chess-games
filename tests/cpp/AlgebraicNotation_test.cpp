#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"r1bqkb1r/pppppppp/2n5/8/4n1Q1/2N5/PPPP1PPP/R1B1KBNR w KQkq - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Knight, Color::White},
				Square::fromRankAndFile(3, 'c'),
				Square::fromRankAndFile(5, 'b')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Nb5");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureNoCheckNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"r1bqkb1r/pppppppp/2n5/8/2n1P1Q1/2N5/PPP2PPP/R1B1KBNR w KQkq - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Bishop, Color::White},
				Square::fromRankAndFile(1, 'f'),
				Square::fromRankAndFile(4, 'c')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Bxc4");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"q1q5/q4k2/2P5/3r4/2P1B3/5K2/Q7/8 b - - 1 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Queen, Color::Black},
				Square::fromRankAndFile(8, 'c'),
				Square::fromRankAndFile(3, 'h')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Qh3+");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureCheckNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"q7/1P3k2/8/3r4/2P1B2q/5K2/Q7/8 b - - 1 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Queen, Color::Black},
				Square::fromRankAndFile(4, 'h'),
				Square::fromRankAndFile(4, 'e')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Qxe4+");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckMateNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"4k3/R6R/8/8/8/8/8/4K3 w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Rook, Color::White},
				Square::fromRankAndFile(7, 'h'),
				Square::fromRankAndFile(8, 'h')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Rh8#");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/4k3/8/8/8/6K1/8/R6R w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Rook, Color::White},
				Square::fromRankAndFile(1, 'h'),
				Square::fromRankAndFile(1, 'd')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Rhd1");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankNoAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/4k3/8/8/8/6K1/8/R6R w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Rook, Color::White},
				Square::fromRankAndFile(1, 'h'),
				Square::fromRankAndFile(2, 'h')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Rh2");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureNoCheckSameFileAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"b4k2/8/2P5/8/b7/8/8/5K2 b - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Bishop, Color::Black},
				Square::fromRankAndFile(8, 'a'),
				Square::fromRankAndFile(6, 'c')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "B8xc6");
}

TEST(AlgebraicNotationTest, PieceMoveCaptureCheckSameFileSameRankAmbiguity) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"b3bk2/8/2P5/8/b7/5K2/8/8 b - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Bishop, Color::Black},
				Square::fromRankAndFile(8, 'a'),
				Square::fromRankAndFile(6, 'c')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Ba8xc6+");
}

TEST(AlgebraicNotationTest, PawnPromotionNoCaptureNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::pawnPromotion(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(7, 'b'),
				Square::fromRankAndFile(8, 'b'),
				PieceType::Queen));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "b8=Q");
}

TEST(AlgebraicNotationTest, PawnPromotionCaptureCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::pawnPromotion(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(7, 'b'),
				Square::fromRankAndFile(8, 'c'),
				PieceType::Rook));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "xc8=R+");
}

TEST(AlgebraicNotationTest, PawnRegularMoveCaptureAmbiguityNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"k7/8/8/3p1p2/4N3/8/8/7K b - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromRankAndFile(5, 'd'),
				Square::fromRankAndFile(4, 'e')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "dxe4");
}

TEST(AlgebraicNotationTest, PawnRegularMoveCaptureNoAmbiguityNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"k7/8/8/6pp/7N/8/8/7K b - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromRankAndFile(5, 'g'),
				Square::fromRankAndFile(4, 'h')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "xh4");
}

TEST(AlgebraicNotationTest, PawnEnPassantCaptureNoAmbiguityNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"7k/8/8/Pp6/8/7K/8/8 w - b6 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(5, 'a'),
				Square::fromRankAndFile(6, 'b')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "xb6");
}

TEST(AlgebraicNotationTest, PawnEnPassantCaptureAmbiguityNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"7k/8/8/PpP5/8/7K/8/8 w - b6 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromRankAndFile(5, 'a'),
				Square::fromRankAndFile(6, 'b')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "axb6");
}

TEST(AlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguityDrawOffer) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/8/3K4/8/Q7/8/p7/1k6 w - - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::Queen, Color::White},
				Square::fromRankAndFile(4, 'a'),
				Square::fromRankAndFile(4, 'b')), true);

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "Qb4+(=)");
}

TEST(AlgebraicNotationTest, CastlingKingsideNoCheck) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"8/8/8/8/6k1/8/4PP1P/4K2R w K - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::King, Color::White},
				Square::fromRankAndFile(1, 'e'),
				Square::fromRankAndFile(1, 'g')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "O-O");
}

TEST(AlgebraicNotationTest, CastlingQueensideCheckmate) {
	const GameManager gameMgr;
	const Game game = gameMgr.createGameFromFen(
			"r3k1K1/1q6/8/8/8/8/8/8 b q - 0 1");

	const auto updatedGame = gameMgr.makeMove(
			game,
			PieceMove::regularMove(
				{PieceType::King, Color::Black},
				Square::fromRankAndFile(8, 'e'),
				Square::fromRankAndFile(8, 'c')));

	EXPECT_EQ(updatedGame.history().back().second.inAlgebraicNotation(), "O-O-O#");
}
