/**
 * Perft ("performance test") move-path enumeration.
 *
 * Perft walks the full legal move tree to a given depth and counts the leaf
 * nodes. The counts below are the published reference values for well-known
 * test positions, so any discrepancy pins down a move generation bug - a
 * missing move, an illegal move allowed through, or mishandled castling
 * rights / en passant / promotion.
 *
 * This is the safety net for optimisation work on the move generator: the
 * numbers must stay identical no matter how the internals are rewritten.
 *
 * The tree is walked through the internal MoveValidator/GameStageUpdater
 * rather than through simplechess::makeMove, because makeMove additionally
 * performs draw evaluation on every node, which is irrelevant here and
 * dominates the runtime.
 */

#include "details/MoveValidator.h"
#include "details/GameStageUpdater.h"
#include "details/fen/FenUtils.h"

#include <gtest/gtest.h>

#include <cstdint>

using namespace simplechess;

namespace
{
	uint64_t perft(const GameStage& stage, const unsigned depth)
	{
		const std::set<PieceMove> moves
			= details::MoveValidator::allAvailableMoves(
					stage.board(),
					stage.enPassantTarget(),
					stage.castlingRights(),
					stage.activeColor());

		if (depth <= 1)
		{
			return moves.size();
		}

		uint64_t nodes = 0;
		for (const auto& move : moves)
		{
			nodes += perft(
					details::GameStageUpdater::makeMove(stage, move),
					depth - 1);
		}

		return nodes;
	}

	uint64_t perft(const std::string& fen, const unsigned depth)
	{
		return perft(details::FenUtils::fromFenString(fen), depth);
	}

	// The standard perft reference positions.
	const std::string START
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	// "Kiwipete": dense middlegame, exercises castling and pins heavily.
	const std::string KIWIPETE
		= "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

	// Sparse endgame, exercises en passant and rook pins along ranks.
	const std::string ENDGAME
		= "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";

	// Exercises promotion (including under-promotion) and promotion captures.
	const std::string PROMOTION
		= "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

	// Known to catch bugs in en passant legality when the capture would
	// expose the king.
	const std::string TRICKY
		= "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";

	const std::string BALANCED
		= "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
}

TEST(PerftTest, StartPosition) {
	EXPECT_EQ(perft(START, 1), 20u);
	EXPECT_EQ(perft(START, 2), 400u);
	EXPECT_EQ(perft(START, 3), 8902u);
}

TEST(PerftTest, Kiwipete) {
	EXPECT_EQ(perft(KIWIPETE, 1), 48u);
	EXPECT_EQ(perft(KIWIPETE, 2), 2039u);
	EXPECT_EQ(perft(KIWIPETE, 3), 97862u);
}

TEST(PerftTest, Endgame) {
	EXPECT_EQ(perft(ENDGAME, 1), 14u);
	EXPECT_EQ(perft(ENDGAME, 2), 191u);
	EXPECT_EQ(perft(ENDGAME, 3), 2812u);
	EXPECT_EQ(perft(ENDGAME, 4), 43238u);
}

TEST(PerftTest, Promotion) {
	EXPECT_EQ(perft(PROMOTION, 1), 6u);
	EXPECT_EQ(perft(PROMOTION, 2), 264u);
	EXPECT_EQ(perft(PROMOTION, 3), 9467u);
}

TEST(PerftTest, Tricky) {
	EXPECT_EQ(perft(TRICKY, 1), 44u);
	EXPECT_EQ(perft(TRICKY, 2), 1486u);
	EXPECT_EQ(perft(TRICKY, 3), 62379u);
}

TEST(PerftTest, Balanced) {
	EXPECT_EQ(perft(BALANCED, 1), 46u);
	EXPECT_EQ(perft(BALANCED, 2), 2079u);
	EXPECT_EQ(perft(BALANCED, 3), 89890u);
}

// Deeper counts, around 15.6 million positions in total. These are the counts
// that catch the rare cases, so they are worth having in the default suite.
TEST(PerftTest, Deep) {
	EXPECT_EQ(perft(START, 4), 197281u);
	EXPECT_EQ(perft(START, 5), 4865609u);
	EXPECT_EQ(perft(KIWIPETE, 4), 4085603u);
	EXPECT_EQ(perft(ENDGAME, 5), 674624u);
	EXPECT_EQ(perft(PROMOTION, 4), 422333u);
	EXPECT_EQ(perft(TRICKY, 4), 2103487u);
	EXPECT_EQ(perft(BALANCED, 4), 3894594u);
}
