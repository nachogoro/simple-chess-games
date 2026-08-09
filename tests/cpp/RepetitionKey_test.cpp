/**
 * The draw evaluator's threefold-repetition look-ahead needs the repetition
 * key of every position reachable in one move. Building a whole GameStage to
 * read that key back out of its FEN is enormously more expensive than
 * assembling the key directly, so it assembles it directly.
 *
 * That shortcut is only valid while the two agree exactly - including the
 * rule that an en passant target is only recorded when the capture is
 * actually available. These tests hold them to that.
 */

#include "details/BoardAnalyzer.h"
#include "details/GameStageUpdater.h"
#include "details/MoveValidator.h"
#include "details/fen/FenUtils.h"

#include <gtest/gtest.h>

using namespace simplechess;

namespace
{
	void expectKeysAgreeForEveryMove(const std::string& fen)
	{
		const GameStage stage = details::FenUtils::fromFenString(fen);

		const std::set<PieceMove> moves = details::MoveValidator::allAvailableMoves(
				stage.board(),
				stage.enPassantTarget(),
				stage.castlingRights(),
				stage.activeColor());

		ASSERT_FALSE(moves.empty()) << "no moves to check in " << fen;

		for (const PieceMove& move : moves)
		{
			// The reference: build the stage in full and cut its FEN down.
			const GameStage nextStage
				= details::GameStageUpdater::makeMove(stage, move, false);
			const std::string expected
				= details::FenUtils::fenForRepetitions(nextStage.fen());

			// The shortcut the draw evaluator actually takes.
			const Board nextBoard
				= details::BoardAnalyzer::makeMoveOnBoard(stage.board(), move);
			const std::string actual = details::FenUtils::repetitionKey(
					nextBoard,
					oppositeColor(stage.activeColor()),
					details::updatedCastlingRights(stage.castlingRights(), move),
					details::MoveValidator::enPassantTarget(nextBoard, move));

			EXPECT_EQ(actual, expected)
				<< "disagreement after " << move.src().toString()
				<< move.dst().toString() << " from " << fen;
		}
	}
}

TEST(RepetitionKeyTest, StartPosition) {
	expectKeysAgreeForEveryMove(
			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST(RepetitionKeyTest, CastlingRightsAreLost) {
	// Rook and king moves here give up castling rights, which are part of the
	// key.
	expectKeysAgreeForEveryMove(
			"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
}

TEST(RepetitionKeyTest, DoublePawnPushesWithCapturerPresent) {
	// Black pawns sit beside the squares White's pawns can double-push to, so
	// an en passant target must be recorded for some pushes and not others.
	expectKeysAgreeForEveryMove("4k3/8/8/8/1p1p1p2/8/P1P1P1P1/4K3 w - - 0 1");
}

TEST(RepetitionKeyTest, DoublePawnPushWithPinnedCapturer) {
	// The black pawn on d4 is the only piece that could capture en passant,
	// and it is pinned against its king, so no en passant target may be
	// recorded even though the pawns are adjacent.
	expectKeysAgreeForEveryMove("8/8/8/8/1k1pP2R/8/8/4K3 b - - 0 1");
	expectKeysAgreeForEveryMove("4k3/8/8/8/3p4/8/2P5/K6R w - - 0 1");
}

TEST(RepetitionKeyTest, EnPassantTargetAlreadyPresent) {
	expectKeysAgreeForEveryMove(
			"rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
}

TEST(RepetitionKeyTest, PromotionsAndCaptures) {
	expectKeysAgreeForEveryMove(
			"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
}

TEST(RepetitionKeyTest, EndgameWithNoCastlingRights) {
	expectKeysAgreeForEveryMove("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
}
