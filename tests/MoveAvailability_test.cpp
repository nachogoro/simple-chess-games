#include <gtest/gtest.h>
#include "Game.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(MoveAvailabilityTest, RegularGameMoves) {
	const Game game = Game::createNewGame();

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'a'),
				Square::instantiateWithRankAndFile(3, 'a')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'a'),
				Square::instantiateWithRankAndFile(4, 'a')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'b'),
				Square::instantiateWithRankAndFile(3, 'b')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'b'),
				Square::instantiateWithRankAndFile(4, 'b')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'c'),
				Square::instantiateWithRankAndFile(3, 'c')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'c'),
				Square::instantiateWithRankAndFile(4, 'c')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'd'),
				Square::instantiateWithRankAndFile(3, 'd')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'd'),
				Square::instantiateWithRankAndFile(4, 'd')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'e'),
				Square::instantiateWithRankAndFile(3, 'e')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'e'),
				Square::instantiateWithRankAndFile(4, 'e')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'f'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'f'),
				Square::instantiateWithRankAndFile(4, 'f')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'g'),
				Square::instantiateWithRankAndFile(3, 'g')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'g'),
				Square::instantiateWithRankAndFile(4, 'g')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'h'),
				Square::instantiateWithRankAndFile(3, 'h')),

		PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(2, 'h'),
				Square::instantiateWithRankAndFile(4, 'h')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'b'),
				Square::instantiateWithRankAndFile(3, 'a')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'b'),
				Square::instantiateWithRankAndFile(3, 'c')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'g'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'g'),
				Square::instantiateWithRankAndFile(3, 'h')),
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, KnightMovesUnobstructed) {
	const Game game = Game::createGameFromStartingFen(
			"7k/8/8/8/3N4/8/8/K7 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'a')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'b')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'c')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'e')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'f')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'c')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'e'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, KnightMovesObstructedAttempt) {
	// This test proves that the knight is unaffected by pieces "on its path"
	// to its destination squares
	const Game game = Game::createGameFromStartingFen(
			"7k/8/8/2rrr3/2rNr3/2rrr3/8/K7 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'a')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'b')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'c')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'e')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'f')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'b')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'c')),

		PieceMove::regularMove(
				{TYPE_KNIGHT, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'e'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, BishopMovesUnobstructed) {
	const Game game = Game::createGameFromStartingFen(
			"3k4/8/8/3BB3/8/8/8/3K4 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(1, 'c')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(2, 'c')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(2, 'e')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'd'),
				Square::instantiateWithRankAndFile(1, 'e')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'a')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'e')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'f')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'g')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'a')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'e')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'g')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(1, 'h')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(6, 'd')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(7, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(8, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(6, 'f')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(7, 'g')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(8, 'h')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(4, 'd')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(3, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(2, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(1, 'a')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(4, 'f')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(3, 'g')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(2, 'h')),
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, BishopMovesObstructed) {
	const Game game = Game::createGameFromStartingFen(
			"7k/r5r1/3r4/8/1r1B2r1/8/1r3r2/2K5 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'c'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(7, 'a')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'e')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'f')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(7, 'g')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'c')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'b')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'e')),

		PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'f'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, RookMovesUnobstructed) {
	const Game game = Game::createGameFromStartingFen(
			"4k3/8/8/3R4/8/8/8/4K3 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'e')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'f')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'f')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'a')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'b')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'c')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'e')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'f')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'g')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'h')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(1, 'd'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, RookMovesObstructed) {
	const Game game = Game::createGameFromStartingFen(
			"7k/r5r1/3r4/8/1r1R2r1/8/1r3r2/2K5 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'c'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'b')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'c')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'e')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'f')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'g')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(1, 'd'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, QueenMovesUnobstructed) {
	const Game game = Game::createGameFromStartingFen(
			"4k3/8/8/3Q4/8/8/8/4K3 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'e')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'f')),

		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'a')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'g')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'a')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'g')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(1, 'h')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'a')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'g')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(5, 'h')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(8, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(7, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(6, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(4, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(3, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'd'),
				Square::instantiateWithRankAndFile(1, 'd'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, QueenMovesObstructed) {
	const Game game = Game::createGameFromStartingFen(
			"7k/r5r1/3r4/8/1r1Q2r1/8/1r3r2/2K5 w - - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'c'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(4, 'g')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(1, 'd')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(7, 'a')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(5, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(6, 'f')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(7, 'g')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'c')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'b')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(3, 'e')),

		PieceMove::regularMove(
				{TYPE_QUEEN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(4, 'd'),
				Square::instantiateWithRankAndFile(2, 'f'))
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, WhiteCastlingUnobstructed) {
	const Game game = Game::createGameFromStartingFen(
			"1k6/8/8/8/8/8/8/R3K2R w KQ - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(3, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(4, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(5, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(6, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(7, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(8, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'b')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'c')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(2, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(3, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(4, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(5, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(6, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(7, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(8, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'g')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'd')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'e')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'c')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'g')),
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, WhiteCastlingQueensideObstructed) {
	const Game game = Game::createGameFromStartingFen(
			"1k6/8/8/6b1/8/8/8/R3K2R w KQ - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(3, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(4, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(5, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(6, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(7, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(8, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'b')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'c')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(2, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(3, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(4, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(5, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(6, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(7, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(8, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'g')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'e')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'g')),
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, WhiteCastlingBothObstructed) {
	const Game game = Game::createGameFromStartingFen(
			"1k6/8/8/6q1/8/8/8/R3K2R w KQ - 0 1");

	const std::set<PieceMove> expectedAvailableMoves =
	{
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(2, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(3, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(4, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(5, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(6, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(7, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(8, 'a')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'b')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'c')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'a'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(2, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(3, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(4, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(5, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(6, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(7, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(8, 'h')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'g')),
		PieceMove::regularMove(
				{TYPE_ROOK, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'h'),
				Square::instantiateWithRankAndFile(1, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'd')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'e')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(2, 'f')),
		PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'f')),
	};

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.size(), expectedAvailableMoves.size());
	EXPECT_EQ(availableMoves, expectedAvailableMoves);
}

TEST(MoveAvailabilityTest, WhiteCastlingUnavailable) {
	const Game game = Game::createGameFromStartingFen(
			"1k6/8/8/8/8/8/8/R3K2R w - - 0 1");

	const PieceMove kingSideCastling = PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'g'));

	const PieceMove queenSideCastling = PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::instantiateWithRankAndFile(1, 'e'),
				Square::instantiateWithRankAndFile(1, 'c'));

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.count(kingSideCastling), 0);
	EXPECT_EQ(availableMoves.count(queenSideCastling), 0);
}

TEST(MoveAvailabilityTest, BlackCastlingInCheck) {
	const Game game = Game::createGameFromStartingFen(
			"4k2r/8/8/8/8/2K5/8/4R3 b k - 0 1");

	const PieceMove kingSideCastling = PieceMove::regularMove(
				{TYPE_KING, COLOR_BLACK},
				Square::instantiateWithRankAndFile(8, 'e'),
				Square::instantiateWithRankAndFile(8, 'g'));

	const PieceMove queenSideCastling = PieceMove::regularMove(
				{TYPE_KING, COLOR_BLACK},
				Square::instantiateWithRankAndFile(8, 'e'),
				Square::instantiateWithRankAndFile(8, 'c'));

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.count(kingSideCastling), 0);
	EXPECT_EQ(availableMoves.count(queenSideCastling), 0);
}

TEST(MoveAvailabilityTest, EnPassantAvailable) {
	const Game game = Game::createGameFromStartingFen(
			"rnbqkbnr/pppp1ppp/8/8/4pP2/4P3/PPPP2PP/RNBQKBNR b KQkq f3 0 1");

	const PieceMove enPassant = PieceMove::regularMove(
				{TYPE_PAWN, COLOR_BLACK},
				Square::instantiateWithRankAndFile(4, 'e'),
				Square::instantiateWithRankAndFile(3, 'f'));

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.count(enPassant), 1);
}

TEST(MoveAvailabilityTest, EnPassantWouldLeaveInCheck) {
	const Game game = Game::createGameFromStartingFen(
			"2k5/6b1/8/3pP3/8/8/1K6/8 w - d6 0 1");

	const PieceMove enPassant = PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(5, 'e'),
				Square::instantiateWithRankAndFile(6, 'd'));

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.count(enPassant), 0);
}

TEST(MoveAvailabilityTest, PawnPromotion) {
	const Game game = Game::createGameFromStartingFen(
			"2k1n3/5Pb1/8/3p4/8/K7/8/8 w - - 0 1");

	const PieceMove promotionQueen = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_QUEEN);

	const PieceMove promotionRook = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_ROOK);

	const PieceMove promotionKnight = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_KNIGHT);

	const PieceMove promotionBishop = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_BISHOP);

	const PieceMove wrongPromotionKing = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_KING);

	const PieceMove wrongPromotionPawn = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'f'),
				TYPE_KING);

	const PieceMove promotionQueenWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_QUEEN);

	const PieceMove promotionRookWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_ROOK);

	const PieceMove promotionKnightWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_KNIGHT);

	const PieceMove promotionBishopWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_BISHOP);

	const PieceMove wrongPromotionKingWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_KING);

	const PieceMove wrongPromotionPawnWithCapture = PieceMove::pawnPromotion(
				{TYPE_PAWN, COLOR_WHITE},
				Square::instantiateWithRankAndFile(7, 'f'),
				Square::instantiateWithRankAndFile(8, 'e'),
				TYPE_KING);

	const std::set<PieceMove> availableMoves = game.allAvailableMoves();
	EXPECT_EQ(availableMoves.count(promotionQueen), 1);
	EXPECT_EQ(availableMoves.count(promotionQueenWithCapture), 1);
	EXPECT_EQ(availableMoves.count(promotionRook), 1);
	EXPECT_EQ(availableMoves.count(promotionRookWithCapture), 1);
	EXPECT_EQ(availableMoves.count(promotionKnight), 1);
	EXPECT_EQ(availableMoves.count(promotionKnightWithCapture), 1);
	EXPECT_EQ(availableMoves.count(promotionBishop), 1);
	EXPECT_EQ(availableMoves.count(promotionBishopWithCapture), 1);
	EXPECT_EQ(availableMoves.count(wrongPromotionKing), 0);
	EXPECT_EQ(availableMoves.count(wrongPromotionKingWithCapture), 0);
	EXPECT_EQ(availableMoves.count(wrongPromotionPawn), 0);
	EXPECT_EQ(availableMoves.count(wrongPromotionPawnWithCapture), 0);
}
