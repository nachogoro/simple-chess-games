#include "TestUtils.h"

// Helper function for checking the FEN a move produces
static void expect_fen_after_move(const char* fen,
                                  simple_chess_piece_type_t piece_type, simple_chess_color_t color,
                                  uint8_t src_rank, char src_file,
                                  uint8_t dst_rank, char dst_file,
                                  const char* expected_fen) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(piece_type, color, src_rank, src_file, dst_rank, dst_file);
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_STREQ(current_stage(updated_game).fen, expected_fen);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

// Helper function for checking the FEN a promotion produces
static void expect_fen_after_promotion(const char* fen, simple_chess_color_t color,
                                       uint8_t src_rank, char src_file,
                                       uint8_t dst_rank, char dst_file,
                                       simple_chess_piece_type_t promoted_to,
                                       const char* expected_fen) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_promotion_move(color, src_rank, src_file, dst_rank, dst_file, promoted_to);
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_STREQ(current_stage(updated_game).fen, expected_fen);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CFenGenerationTest, BlackMoveNoCapture) {
    expect_fen_after_move(
        "rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR b KQkq - 3 5",
        SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 8, 'd', 6, 'f',
        "rnb1kbnr/ppp2ppp/5q2/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR w KQkq - 4 6");
}

TEST(CFenGenerationTest, BlackMoveWithCapture) {
    expect_fen_after_move(
        "8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 b - - 15 26",
        SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 4, 'h', 6, 'h',
        "8/pB1K4/7q/8/1RnP1P2/4P3/4k3/8 w - - 0 27");
}

TEST(CFenGenerationTest, BlackPawnMove) {
    expect_fen_after_promotion(
        "8/1B1K4/7N/8/1RnP1P1q/4P3/p3k3/8 b - - 15 26",
        SIMPLE_CHESS_COLOR_BLACK, 2, 'a', 1, 'a', SIMPLE_CHESS_PIECE_TYPE_QUEEN,
        "8/1B1K4/7N/8/1RnP1P1q/4P3/4k3/q7 w - - 0 27");
}

TEST(CFenGenerationTest, WhiteMoveNoCapture) {
    expect_fen_after_move(
        "rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR w KQkq - 3 5",
        SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'b', 3, 'c',
        "rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/2NP4/PPP2PPP/R2QKBNR b KQkq - 4 5");
}

TEST(CFenGenerationTest, WhiteMoveWithCapture) {
    expect_fen_after_move(
        "8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 w - - 1 30",
        SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 4, 'b', 4, 'c',
        "8/pB1K4/7N/8/2RP1P1q/4P3/4k3/8 b - - 0 30");
}

TEST(CFenGenerationTest, WhitePawnMove) {
    expect_fen_after_move(
        "8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 w - - 12 29",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 4, 'f', 5, 'f',
        "8/pB1K4/7N/5P2/1RnP3q/4P3/4k3/8 b - - 0 29");
}

TEST(CFenGenerationTest, EnPassantTaken) {
    expect_fen_after_move(
        "rnbqkbnr/pppp1ppp/8/8/3Pp3/2N5/PPP1PPPP/R1BQKBNR b KQkq d3 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 4, 'e', 3, 'd',
        "rnbqkbnr/pppp1ppp/8/8/8/2Np4/PPP1PPPP/R1BQKBNR w KQkq - 0 2");
}

TEST(CFenGenerationTest, EnPassantIgnored) {
    expect_fen_after_move(
        "rnbqkbnr/pppp1ppp/8/8/3Pp3/2N5/PPP1PPPP/R1BQKBNR b KQkq d3 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 4, 'e', 3, 'e',
        "rnbqkbnr/pppp1ppp/8/8/3P4/2N1p3/PPP1PPPP/R1BQKBNR w KQkq - 0 2");
}

TEST(CFenGenerationTest, EnPassantIsCreatedWhenEnemyPawnAdjacent) {
    // Black pawn on e4 is adjacent to f4, so en passant target is reported
    expect_fen_after_move(
        "rnbqkbnr/pppp1ppp/8/8/4p3/8/PPPPPPPP/RNBQKBNR w KQkq - 0 2",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'f', 4, 'f',
        "rnbqkbnr/pppp1ppp/8/8/4pP2/8/PPPPP1PP/RNBQKBNR b KQkq f3 0 2");
}

TEST(CFenGenerationTest, EnPassantNotCreatedWhenNoEnemyPawnAdjacent) {
    // No black pawn adjacent to f4, so no en passant target
    expect_fen_after_move(
        "rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'f', 4, 'f',
        "rnbqkbnr/pppp1ppp/8/4p3/5P2/4P3/PPPP2PP/RNBQKBNR b KQkq - 0 2");
}

TEST(CFenGenerationTest, EnPassantNotCreatedWhenCaptureWouldLeaveInCheck) {
    // Black pawn on e4, black king on c4, white rook on h4.
    // If white plays d2-d4, the black pawn on e4 is adjacent but capturing
    // exd3 would expose the black king to the rook on h4, so no ep target.
    expect_fen_after_move(
        "8/8/8/8/2k1p2R/8/3P4/4K3 w - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'd', 4, 'd',
        "8/8/8/8/2kPp2R/8/8/4K3 b - - 0 1");
}

TEST(CFenGenerationTest, WhiteKingSideCastling) {
    expect_fen_after_move(
        "rn1qkbnr/pp2pppp/2p5/1B1p4/4P1b1/5N2/PPPP1PPP/RNBQK2R w KQkq - 1 6",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g',
        "rn1qkbnr/pp2pppp/2p5/1B1p4/4P1b1/5N2/PPPP1PPP/RNBQ1RK1 b kq - 2 6");
}

TEST(CFenGenerationTest, WhiteQueensideCastling) {
    expect_fen_after_move(
        "rnbq1rk1/ppp2ppp/5n2/3pp3/1b2P3/2NP1Q2/PPPB1PPP/R3KBNR w KQ - 9 10",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'c',
        "rnbq1rk1/ppp2ppp/5n2/3pp3/1b2P3/2NP1Q2/PPPB1PPP/2KR1BNR b - - 10 10");
}

TEST(CFenGenerationTest, BlackKingsideCastling) {
    expect_fen_after_move(
        "3rk2r/8/8/8/8/8/8/3RK2R b Kk - 0 41",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'e', 8, 'g',
        "3r1rk1/8/8/8/8/8/8/3RK2R w K - 1 42");
}

TEST(CFenGenerationTest, BlackQueensideCastling) {
    expect_fen_after_move(
        "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 7 52",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'e', 8, 'c',
        "2kr3r/8/8/8/8/8/8/R3K2R w KQ - 8 53");
}
