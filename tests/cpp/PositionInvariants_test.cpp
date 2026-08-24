/**
 * Properties which hold of any legal position, checked over a tree of them.
 *
 * These need no expected values, so they are checked at every node:
 *
 *   - No two legal moves may share a notation.
 *   - A FEN survives being parsed and written back out.
 *   - A player with no legal move is not still playing.
 *
 * Two ply, which is the shallowest depth at which kiwipete's knights collide,
 * and about forty times the cost of a perft node, since rendering a move
 * generates the move list again.
 */

#include "details/AlgebraicNotationGenerator.h"
#include "details/GameStageUpdater.h"
#include "details/GameStateDetector.h"
#include "details/MoveValidator.h"
#include "details/fen/FenUtils.h"

#include <gtest/gtest.h>

#include <map>
#include <string>
#include <vector>

using namespace simplechess;

namespace
{
	struct Position
	{
		const char* name;
		const char* fen;
	};

	// Perft's positions, for the same reason it uses them: between them they
	// exercise castling, promotion, en passant and pins.
	const Position POSITIONS[] = {
		{"start", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
		// Two black knights, on b6 and f6, both reach d5 and d7
		{"kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"},
		{"endgame", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"},
		{"promotion", "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"},
		{"tricky", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"},
		{"balanced", "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"},

		// The six above are all mid-game, so on their own they would never
		// reach a finished position and the terminal-state property would go
		// unexercised. These end within the depth walked.
		{"mate in one", "6k1/5ppp/8/8/8/8/8/R5K1 w - - 0 1"},
		{"stalemated", "7k/5Q2/6K1/8/8/8/8/8 b - - 0 1"},
		{"lone bishop", "3k4/8/8/8/8/8/8/2B3K1 w - - 0 1"},
	};

	const unsigned DEPTH = 2;

	std::string describe(const PieceMove& move)
	{
		std::string description
			= move.src().toString() + move.dst().toString();

		if (move.promoted())
		{
			switch (*move.promoted())
			{
				case PieceType::Queen:  description += "=Q"; break;
				case PieceType::Rook:   description += "=R"; break;
				case PieceType::Bishop: description += "=B"; break;
				case PieceType::Knight: description += "=N"; break;
				default:                description += "=?"; break;
			}
		}

		return description;
	}

	std::string where(
			const Position& root,
			const std::string& path,
			const GameStage& stage)
	{
		return std::string(root.name) + " [" + path + "]\n  position: "
			+ stage.fen();
	}

	// The check and mate marks say what a move did rather than which move it
	// was, so everything is rendered as though it gave no check.
	void expectNotationsAreUnique(
			const Position& root,
			const std::string& path,
			const GameStage& stage,
			const std::vector<PieceMove>& moves)
	{
		std::map<std::string, PieceMove> seen;

		for (const PieceMove& move : moves)
		{
			const std::string notation
				= details::AlgebraicNotationGenerator::toAlgebraicNotation(
						stage.board(), move, false, CheckType::NoCheck);

			const auto inserted = seen.emplace(notation, move);

			EXPECT_TRUE(inserted.second)
				<< where(root, path, stage) << "\n  two legal moves share the "
				<< "notation \"" << notation << "\"\n  first:    "
				<< describe(inserted.first->second) << "\n  second:   "
				<< describe(move);
		}
	}

	// Only at this level: a stage carries the en passant target it was given,
	// while a game built from a FEN replays the pawn push which created one
	// and re-derives it, so the two do not agree by design.
	void expectFenSurvivesARoundTrip(
			const Position& root,
			const std::string& path,
			const GameStage& stage)
	{
		const std::string original = stage.fen();
		const std::string reparsed
			= details::FenUtils::fromFenString(original).fen();

		EXPECT_EQ(original, reparsed)
			<< where(root, path, stage) << "\n  parsing and regenerating the "
			<< "FEN did not return it unchanged";
	}

	void expectStateAgreesWithTheMoveList(
			const Position& root,
			const std::string& path,
			const GameStage& stage,
			const details::PositionAnalysis& analysis)
	{
		for (const DrawEnforcement enforcement :
				{DrawEnforcement::Automatic, DrawEnforcement::ClaimOnly})
		{
			const details::GameStateInformation info
				= details::GameStateDetector::detect(
						stage, analysis, false, {}, enforcement);

			if (analysis.legalMoves.empty())
			{
				EXPECT_NE(info.gameState, GameState::Playing)
					<< where(root, path, stage) << "\n  no legal move, but "
					<< "the game is still being played";
			}

			const bool somebodyWon
				= info.gameState == GameState::WhiteWon
					|| info.gameState == GameState::BlackWon;

			EXPECT_EQ(
					somebodyWon,
					analysis.legalMoves.empty() && analysis.inCheck())
				<< where(root, path, stage) << "\n  a game is won exactly "
				<< "when the player to move is in check and has no move";
		}
	}

	void walk(
			const Position& root,
			const GameStage& stage,
			const unsigned depth,
			const std::string& path)
	{
		// A disambiguation fault collides across much of the tree, and an
		// ASSERT would only unwind one frame and leave the caller iterating,
		// so the walk stops itself at the first failure.
		if (::testing::Test::HasFailure())
		{
			return;
		}

		const details::PositionAnalysis analysis = details::analyzePosition(
				stage.board(),
				stage.activeColor(),
				stage.enPassantTarget(),
				stage.castlingRights());

		expectNotationsAreUnique(root, path, stage, analysis.legalMoves);
		expectFenSurvivesARoundTrip(root, path, stage);
		expectStateAgreesWithTheMoveList(root, path, stage, analysis);

		if (depth == 0)
		{
			return;
		}

		for (const PieceMove& move : analysis.legalMoves)
		{
			walk(
					root,
					details::GameStageUpdater::makeMove(stage, move),
					depth - 1,
					path.empty()
						? describe(move)
						: path + " " + describe(move));
		}
	}

	void walkFromEveryPosition(const unsigned depth)
	{
		for (const Position& position : POSITIONS)
		{
			walk(
					position,
					details::FenUtils::fromFenString(position.fen),
					depth,
					"");
		}
	}
}

TEST(PositionInvariantsTest, EveryPositionInTheTreeHoldsThem) {
	walkFromEveryPosition(DEPTH);
}
