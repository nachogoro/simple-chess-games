#include "TestUtils.h"

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "r1bqkb1r/pppppppp/2n5/8/4n1Q1/2N5/PPPP1PPP/R1B1KBNR w KQkq - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 3, 'c', 5, 'b');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Nb5");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureNoCheckNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "r1bqkb1r/pppppppp/2n5/8/2n1P1Q1/2N5/PPP2PPP/R1B1KBNR w KQkq - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 1, 'f', 4, 'c');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Bxc4");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "q1q5/q4k2/2P5/3r4/2P1B3/5K2/Q7/8 b - - 1 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 8, 'c', 3, 'h');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qh3+");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureCheckNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "q7/1P3k2/8/3r4/2P1B2q/5K2/Q7/8 b - - 1 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 4, 'h', 4, 'e');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qxe4+");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckMateNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "4k3/R6R/8/8/8/8/8/4K3 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 7, 'h', 8, 'h');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rh8#");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/8/6K1/8/R6R w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 1, 'h', 1, 'd');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rhd1");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankNoAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/8/6K1/8/R6R w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 1, 'h', 2, 'h');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rh2");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureNoCheckSameFileAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "b4k2/8/2P5/8/b7/8/8/5K2 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_BLACK, 8, 'a', 6, 'c');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "B8xc6");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureCheckSameFileSameRankAmbiguity) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "b3bk2/8/2P5/8/b7/5K2/8/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_BLACK, 8, 'a', 6, 'c');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Ba8xc6+");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckDifferentRankAndFileAmbiguity) {
    // Position after 11. Bxb5+ in Morphy's Opera Game: either black knight can
    // block on d7, and they share neither rank nor file.
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "rn2kb1r/p3qppp/5n2/1B2p1B1/4P3/1Q6/PPP2PPP/R3K2R b KQkq - 0 11",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 8, 'b', 7, 'd');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Nbd7");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnPromotionNoCaptureNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'b', 8, 'b', SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "b8=Q");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnPromotionCaptureCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'b', 8, 'c', SIMPLE_CHESS_PIECE_TYPE_ROOK);
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "bxc8=R+");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnRegularMoveCaptureAmbiguityNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "k7/8/8/3p1p2/4N3/8/8/7K b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 5, 'd', 4, 'e');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "dxe4");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnRegularMoveCaptureNoAmbiguityNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "k7/8/8/6pp/7N/8/8/7K b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 5, 'g', 4, 'h');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "gxh4");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnEnPassantCaptureNoAmbiguityNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/Pp6/8/7K/8/8 w - b6 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 5, 'a', 6, 'b');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "axb6");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnEnPassantCaptureAmbiguityNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/PpP5/8/7K/8/8 w - b6 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 5, 'a', 6, 'b');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "axb6");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguityDrawOffer) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/8/3K4/8/Q7/8/p7/1k6 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'a', 4, 'b');
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(game, move, true, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qb4+(=)");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, CastlingKingsideNoCheck) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/8/8/8/6k1/8/4PP1P/4K2R w K - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "O-O");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, CastlingQueensideCheckmate) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "r3k1K1/1q6/8/8/8/8/8/8 b q - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t move = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'e', 8, 'c');
    simple_chess_game_t* updated_game = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "O-O-O#");

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(updated_game);
}
