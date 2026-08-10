/**
 * Benchmark and golden-trace driver for simple-chess-games.
 *
 * Not part of the installed library. Build with -DBUILD_BENCHMARKS=ON and,
 * for meaningful numbers, -DCMAKE_BUILD_TYPE=Release.
 *
 * Two modes:
 *
 *   bench_simplechess            Run the timing workloads.
 *   bench_simplechess golden     Emit a deterministic trace of everything the
 *                                public API reports over a set of games.
 *                                Capture it before and after a change and
 *                                diff: it must be byte-identical.
 */

#include <simplechess-c/simplechess.h>
#include <simplechess/SimpleChess.h>

#include <chrono>
#include <cstdint>
#include <istream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace simplechess;

namespace
{
	/**
	 * A fixed linear congruential generator, so every run plays exactly the
	 * same games regardless of platform or standard library.
	 */
	class Lcg
	{
		public:
			explicit Lcg(const uint64_t seed) : mState(seed) {}

			uint64_t next()
			{
				mState = mState * 6364136223846793005ULL + 1442695040888963407ULL;
				return mState >> 33;
			}

			size_t below(const size_t bound)
			{
				return static_cast<size_t>(next() % bound);
			}

		private:
			uint64_t mState;
	};

	struct GameResult
	{
		Game game;
		unsigned plies;
	};

	/**
	 * Plays a deterministic pseudo-random game, stopping when the game ends
	 * or after \p maxPlies half-moves.
	 */
	GameResult playRandomGame(
			const uint64_t seed,
			const unsigned maxPlies,
			const DrawEnforcement enforcement = DrawEnforcement::Automatic)
	{
		Lcg rng(seed);
		Game game = createNewGame(enforcement);
		unsigned plies = 0;

		while (plies < maxPlies && game.gameState() == GameState::Playing)
		{
			const std::vector<PieceMove>& moves = game.allAvailableMoves();
			if (moves.empty())
			{
				break;
			}

			game = makeMove(game, moves[rng.below(moves.size())]);
			++plies;
		}

		return {game, plies};
	}

	/**
	 * Shuffles both sides' knights out and back repeatedly. Every four plies
	 * the starting position recurs, which is the worst case for repetition
	 * bookkeeping and for the threefold-repetition lookahead.
	 *
	 * ClaimOnly enforcement keeps the game alive past the fivefold repetition
	 * that would otherwise end it automatically.
	 */
	GameResult playShufflingGame(const unsigned maxPlies)
	{
		const Piece whiteKnight = {PieceType::Knight, Color::White};
		const Piece blackKnight = {PieceType::Knight, Color::Black};

		const std::vector<PieceMove> cycle = {
			PieceMove::regularMove(whiteKnight,
					Square::fromRankAndFile(1, 'g'), Square::fromRankAndFile(3, 'f')),
			PieceMove::regularMove(blackKnight,
					Square::fromRankAndFile(8, 'g'), Square::fromRankAndFile(6, 'f')),
			PieceMove::regularMove(whiteKnight,
					Square::fromRankAndFile(3, 'f'), Square::fromRankAndFile(1, 'g')),
			PieceMove::regularMove(blackKnight,
					Square::fromRankAndFile(6, 'f'), Square::fromRankAndFile(8, 'g')),
		};

		Game game = createNewGame(DrawEnforcement::ClaimOnly);
		unsigned plies = 0;

		while (plies < maxPlies && game.gameState() == GameState::Playing)
		{
			game = makeMove(game, cycle[plies % cycle.size()]);
			++plies;
		}

		return {game, plies};
	}

	/**
	 * The same deterministic game as \ref playRandomGame, driven through the
	 * C API, so that whatever the C boundary costs on top of playing the
	 * game shows up here.
	 */
	unsigned playRandomGameThroughC(const uint64_t seed, const unsigned maxPlies)
	{
		Lcg rng(seed);
		simple_chess_game_t* game = simple_chess_create_new_game(
				SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
		unsigned plies = 0;

		while (game != nullptr
				&& plies < maxPlies
				&& simple_chess_game_state(game) == SIMPLE_CHESS_GAME_STATE_PLAYING
				&& simple_chess_game_available_move_count(game) > 0)
		{
			simple_chess_piece_move_t move;
			if (!simple_chess_game_available_move(
						game,
						static_cast<uint16_t>(rng.below(
							simple_chess_game_available_move_count(game))),
						&move))
			{
				break;
			}

			simple_chess_game_t* next = simple_chess_make_move(game, move, NULL);

			if (next == nullptr)
			{
				break;
			}

			simple_chess_destroy_game(game);
			game = next;
			++plies;
		}

		if (game != nullptr)
		{
			simple_chess_destroy_game(game);
		}

		return plies;
	}

	const std::vector<std::string>& analysisPositions()
	{
		static const std::vector<std::string> positions = {
			"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
			"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
			"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
			"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
		};
		return positions;
	}

	using Clock = std::chrono::steady_clock;

	double millisSince(const Clock::time_point start)
	{
		return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
	}

	void report(
			const std::string& name,
			const double totalMillis,
			const unsigned long operations,
			const std::string& unit)
	{
		std::cout << std::left << std::setw(34) << name
			<< std::right << std::setw(11) << std::fixed << std::setprecision(2)
			<< totalMillis << " ms  "
			<< std::setw(10) << std::setprecision(1)
			<< (totalMillis * 1000.0 / static_cast<double>(operations))
			<< " us/" << unit
			<< "  (" << operations << ")\n";
	}

	int runBenchmarks()
	{
		std::cout << "workload                              total        per-op\n"
		          << "-------------------------------------------------------------------\n";

		// 1. Full games played move by move. The headline number.
		{
			const auto start = Clock::now();
			unsigned long moves = 0;
			for (uint64_t seed = 1; seed <= 5; ++seed)
			{
				moves += playRandomGame(seed, 120).plies;
			}
			report("random games (5 x <=120 plies)", millisSince(start), moves, "move");
		}

		// 2. Repetition-heavy game: drives the repetition bookkeeping and the
		//    threefold lookahead as hard as possible.
		{
			const auto start = Clock::now();
			const unsigned plies = playShufflingGame(60).plies;
			report("knight shuffle (repetitions)", millisSince(start), plies, "move");
		}

		// 3. Cold analysis of a position: parse a FEN and derive everything.
		{
			const unsigned rounds = 40;
			const auto start = Clock::now();
			unsigned long count = 0;
			for (unsigned i = 0; i < rounds; ++i)
			{
				for (const std::string& fen : analysisPositions())
				{
					const Game game = createGameFromFen(fen);
					count += game.allAvailableMoves().size() > 0 ? 1 : 0;
				}
			}
			report("createGameFromFen", millisSince(start), count, "position");
		}

		// 4. Long single game: exposes any per-move cost that grows with the
		//    length of the history.
		{
			const auto start = Clock::now();
			const unsigned plies = playRandomGame(42, 300, DrawEnforcement::ClaimOnly).plies;
			report("single long game", millisSince(start), plies, "move");
		}

		// 5. The same through the C API, which rebuilds the whole game from
		//    its C representation on every call. Cost per move here grows
		//    with the length of the history, so a long game is the point.
		{
			const auto start = Clock::now();
			const unsigned plies = playRandomGameThroughC(42, 200);
			report("single long game (C API)", millisSince(start), plies, "move");
		}

		return 0;
	}

	void dumpStage(const GameStage& stage)
	{
		std::cout << "  fen=" << stage.fen()
			<< " check=" << static_cast<int>(stage.checkStatus())
			<< " castling=" << static_cast<int>(stage.castlingRights())
			<< " half=" << stage.halfMovesSinceLastCaptureOrPawnAdvance()
			<< " full=" << stage.fullMoveCounter()
			<< " ep=" << (stage.enPassantTarget()
					? stage.enPassantTarget()->toString()
					: "-")
			<< "\n";
	}

	void dumpGame(const std::string& label, const Game& game)
	{
		std::cout << "== " << label << "\n";
		std::cout << "state=" << static_cast<int>(game.gameState()) << "\n";
		if (game.gameState() == GameState::Drawn)
		{
			std::cout << "drawReason=" << static_cast<int>(*game.drawReason()) << "\n";
		}
		else if (game.gameState() == GameState::Playing)
		{
			const std::optional<DrawReason>& claim = game.reasonToClaimDraw();
			std::cout << "claimable="
				<< (claim ? std::to_string(static_cast<int>(*claim)) : "-") << "\n";
		}

		std::cout << "history=" << game.history().size() << "\n";
		for (const auto& entry : game.history())
		{
			dumpStage(entry.stage);
			std::cout << "  played=" << entry.move.inAlgebraicNotation()
				<< " check=" << static_cast<int>(entry.move.checkType())
				<< " captured="
				<< (entry.move.capturedPiece()
						? std::to_string(static_cast<int>(entry.move.capturedPiece()->type()))
						: "-")
				<< "\n";
		}

		std::cout << "current:\n";
		dumpStage(game.currentStage());

		if (game.gameState() == GameState::Playing)
		{
			std::cout << "moves=" << game.allAvailableMoves().size() << "\n";
			for (const PieceMove& move : game.allAvailableMoves())
			{
				std::cout << "  " << move.src().toString()
					<< move.dst().toString()
					<< (move.promoted()
							? std::to_string(static_cast<int>(*move.promoted()))
							: "")
					<< "\n";
			}
		}
	}

	int runGolden()
	{
		for (uint64_t seed = 1; seed <= 5; ++seed)
		{
			const GameResult result = playRandomGame(seed, 120);
			dumpGame("random seed " + std::to_string(seed), result.game);
		}

		dumpGame("knight shuffle", playShufflingGame(60).game);

		for (const std::string& fen : analysisPositions())
		{
			dumpGame("fen " + fen, createGameFromFen(fen));
		}

		return 0;
	}
}

int main(const int argc, const char** argv)
{
	const std::string mode = (argc > 1) ? argv[1] : "bench";

	if (mode == "golden")
	{
		return runGolden();
	}

	if (mode == "movedump")
	{
		// Reads FENs on stdin and prints the legal moves of each, so that two
		// builds of the library can be compared position by position.
		std::string fen;
		while (std::getline(std::cin, fen))
		{
			if (fen.empty())
			{
				continue;
			}

			std::cout << fen << "\n";

			try
			{
				const Game game = createGameFromFen(fen, DrawEnforcement::ClaimOnly);
				for (const PieceMove& move : game.allAvailableMoves())
				{
					std::cout << "  " << move.src().toString()
						<< move.dst().toString()
						<< (move.promoted()
								? std::to_string(static_cast<int>(*move.promoted()))
								: "")
						<< "\n";
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "  <rejected>\n";
			}
		}

		return 0;
	}

	if (mode != "bench")
	{
		std::cerr << "usage: " << argv[0] << " [bench|golden|movedump]\n";
		return 1;
	}

	return runBenchmarks();
}
