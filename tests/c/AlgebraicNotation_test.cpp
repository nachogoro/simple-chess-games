#include <gtest/gtest.h>
#include <c/simplechess/chess.h>

// Test fixture for C interface tests
class CAlgebraicNotationTest : public ::testing::Test {
	protected:
		void SetUp() override {
			manager = chess_create_manager();
			ASSERT_NE(manager, nullptr);
		}

		void TearDown() override {
			if (manager) {
				chess_destroy_manager(manager);
			}
		}

		chess_game_manager_t manager = nullptr;

		// Helper to get algebraic notation for the last move
		std::string GetLastMoveAlgebraicNotation(const chess_game_t& game) {
			char notation_buffer[10];
			if (game.history_count == 0) {
				return "";
			}

			bool success = chess_get_move_algebraic_notation(manager, &game, game.history_count - 1, notation_buffer);
			if (!success) {
				return "";
			}

			return std::string(notation_buffer);
		}

		// Helper to create a move
		chess_move_t CreateMove(
				uint8_t from_rank,
				char from_file,
				uint8_t to_rank,
				char to_file,
				chess_piece_type_t piece_type,
				chess_color_t piece_color)
		{
			chess_move_t move;
			move.piece = {piece_type, piece_color};
			move.from = {from_rank, from_file};
			move.to = {to_rank, to_file};
			move.is_promotion = false;
			move.promotion = CHESS_PIECE_QUEEN;
			return move;
		}

		// Helper to create a promotion move
		chess_move_t CreatePromotionMove(
				uint8_t from_rank, char from_file, uint8_t to_rank, char to_file,
				chess_piece_type_t piece_type, chess_color_t piece_color,
				chess_piece_type_t promotion_piece) {
			chess_move_t move;
			move.piece = {piece_type, piece_color};
			move.from = {from_rank, from_file};
			move.to = {to_rank, to_file};
			move.is_promotion = true;
			move.promotion = promotion_piece;
			return move;
		}
};

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"r1bqkb1r/pppppppp/2n5/8/4n1Q1/2N5/PPPP1PPP/R1B1KBNR w KQkq - 0 1", &game));

	chess_move_t move = CreateMove(3, 'c', 5, 'b', CHESS_PIECE_KNIGHT, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));
	EXPECT_EQ(result_game.history_count, 1);

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Nb5");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveCaptureNoCheckNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"r1bqkb1r/pppppppp/2n5/8/2n1P1Q1/2N5/PPP2PPP/R1B1KBNR w KQkq - 0 1", &game));

	chess_move_t move = CreateMove(1, 'f', 4, 'c', CHESS_PIECE_BISHOP, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Bxc4");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"q1q5/q4k2/2P5/3r4/2P1B3/5K2/Q7/8 b - - 1 1", &game));

	chess_move_t move = CreateMove(8, 'c', 3, 'h', CHESS_PIECE_QUEEN, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Qh3+");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveCaptureCheckNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"q7/1P3k2/8/3r4/2P1B2q/5K2/Q7/8 b - - 1 1", &game));

	chess_move_t move = CreateMove(4, 'h', 4, 'e', CHESS_PIECE_QUEEN, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Qxe4+");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureCheckMateNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"4k3/R6R/8/8/8/8/8/4K3 w - - 0 1", &game));

	chess_move_t move = CreateMove(7, 'h', 8, 'h', CHESS_PIECE_ROOK, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Rh8#");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"8/4k3/8/8/8/6K1/8/R6R w - - 0 1", &game));

	chess_move_t move = CreateMove(1, 'h', 1, 'd', CHESS_PIECE_ROOK, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Rhd1");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankNoAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"8/4k3/8/8/8/6K1/8/R6R w - - 0 1", &game));

	chess_move_t move = CreateMove(1, 'h', 2, 'h', CHESS_PIECE_ROOK, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Rh2");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveCaptureNoCheckSameFileAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"b4k2/8/2P5/8/b7/8/8/5K2 b - - 0 1", &game));

	chess_move_t move = CreateMove(8, 'a', 6, 'c', CHESS_PIECE_BISHOP, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "B8xc6");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveCaptureCheckSameFileSameRankAmbiguity) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"b3bk2/8/2P5/8/b7/5K2/8/8 b - - 0 1", &game));

	chess_move_t move = CreateMove(8, 'a', 6, 'c', CHESS_PIECE_BISHOP, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Ba8xc6+");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnPromotionNoCaptureNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1", &game));

	chess_move_t move = CreatePromotionMove(7, 'b', 8, 'b', CHESS_PIECE_PAWN, CHESS_COLOR_WHITE, CHESS_PIECE_QUEEN);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "b8=Q");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnPromotionCaptureCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1", &game));

	chess_move_t move = CreatePromotionMove(7, 'b', 8, 'c', CHESS_PIECE_PAWN, CHESS_COLOR_WHITE, CHESS_PIECE_ROOK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "xc8=R+");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnRegularMoveCaptureAmbiguityNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"k7/8/8/3p1p2/4N3/8/8/7K b - - 0 1", &game));

	chess_move_t move = CreateMove(5, 'd', 4, 'e', CHESS_PIECE_PAWN, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "dxe4");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnRegularMoveCaptureNoAmbiguityNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"k7/8/8/6pp/7N/8/8/7K b - - 0 1", &game));

	chess_move_t move = CreateMove(5, 'g', 4, 'h', CHESS_PIECE_PAWN, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "xh4");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnEnPassantCaptureNoAmbiguityNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"7k/8/8/Pp6/8/7K/8/8 w - b6 0 1", &game));

	chess_move_t move = CreateMove(5, 'a', 6, 'b', CHESS_PIECE_PAWN, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "xb6");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PawnEnPassantCaptureAmbiguityNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"7k/8/8/PpP5/8/7K/8/8 w - b6 0 1", &game));

	chess_move_t move = CreateMove(5, 'a', 6, 'b', CHESS_PIECE_PAWN, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "axb6");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguityDrawOffer) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"8/8/3K4/8/Q7/8/p7/1k6 w - - 0 1", &game));

	chess_move_t move = CreateMove(4, 'a', 4, 'b', CHESS_PIECE_QUEEN, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, true, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "Qb4+(=)");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, CastlingKingsideNoCheck) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"8/8/8/8/6k1/8/4PP1P/4K2R w K - 0 1", &game));

	chess_move_t move = CreateMove(1, 'e', 1, 'g', CHESS_PIECE_KING, CHESS_COLOR_WHITE);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "O-O");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}

TEST_F(CAlgebraicNotationTest, CastlingQueensideCheckmate) {
	chess_game_t game;
	ASSERT_TRUE(chess_create_game_from_fen(manager,
				"r3k1K1/1q6/8/8/8/8/8/8 b q - 0 1", &game));

	chess_move_t move = CreateMove(8, 'e', 8, 'c', CHESS_PIECE_KING, CHESS_COLOR_BLACK);
	chess_game_t result_game;
	ASSERT_TRUE(chess_make_move(manager, &game, &move, false, &result_game));

	EXPECT_EQ(GetLastMoveAlgebraicNotation(result_game), "O-O-O#");

	chess_destroy_game(&game);
	chess_destroy_game(&result_game);
}
