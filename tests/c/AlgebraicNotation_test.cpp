#include "TestUtils.h"

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "r1bqkb1r/pppppppp/2n5/8/4n1Q1/2N5/PPPP1PPP/R1B1KBNR w KQkq - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeKnight, ColorWhite, 3, 'c', 5, 'b');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Nb5");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureNoCheckNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "r1bqkb1r/pppppppp/2n5/8/2n1P1Q1/2N5/PPP2PPP/R1B1KBNR w KQkq - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeBishop, ColorWhite, 1, 'f', 4, 'c');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Bxc4");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "q1q5/q4k2/2P5/3r4/2P1B3/5K2/Q7/8 b - - 1 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeQueen, ColorBlack, 8, 'c', 3, 'h');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qh3+");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureCheckNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "q7/1P3k2/8/3r4/2P1B2q/5K2/Q7/8 b - - 1 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeQueen, ColorBlack, 4, 'h', 4, 'e');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qxe4+");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckMateNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "4k3/R6R/8/8/8/8/8/4K3 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeRook, ColorWhite, 7, 'h', 8, 'h');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rh8#");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/8/6K1/8/R6R w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeRook, ColorWhite, 1, 'h', 1, 'd');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rhd1");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureNoCheckSameRankNoAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/8/6K1/8/R6R w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeRook, ColorWhite, 1, 'h', 2, 'h');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Rh2");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureNoCheckSameFileAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "b4k2/8/2P5/8/b7/8/8/5K2 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeBishop, ColorBlack, 8, 'a', 6, 'c');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "B8xc6");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveCaptureCheckSameFileSameRankAmbiguity) {
    game_t* game = simple_chess_create_game_from_fen(
        "b3bk2/8/2P5/8/b7/5K2/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeBishop, ColorBlack, 8, 'a', 6, 'c');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Ba8xc6+");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnPromotionNoCaptureNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_promotion_move(ColorWhite, 7, 'b', 8, 'b', PieceTypeQueen);
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "b8=Q");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnPromotionCaptureCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "2rk4/1P6/8/5K2/8/8/8/8 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_promotion_move(ColorWhite, 7, 'b', 8, 'c', PieceTypeRook);
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "xc8=R+");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnRegularMoveCaptureAmbiguityNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "k7/8/8/3p1p2/4N3/8/8/7K b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypePawn, ColorBlack, 5, 'd', 4, 'e');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "dxe4");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnRegularMoveCaptureNoAmbiguityNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "k7/8/8/6pp/7N/8/8/7K b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypePawn, ColorBlack, 5, 'g', 4, 'h');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "xh4");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnEnPassantCaptureNoAmbiguityNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/Pp6/8/7K/8/8 w - b6 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypePawn, ColorWhite, 5, 'a', 6, 'b');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "xb6");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PawnEnPassantCaptureAmbiguityNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/PpP5/8/7K/8/8 w - b6 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypePawn, ColorWhite, 5, 'a', 6, 'b');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "axb6");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, PieceMoveNoCaptureCheckNoAmbiguityDrawOffer) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/8/3K4/8/Q7/8/p7/1k6 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeQueen, ColorWhite, 4, 'a', 4, 'b');
    game_t* updated_game = simple_chess_make_move_with_draw_offer(game, move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "Qb4+(=)");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, CastlingKingsideNoCheck) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/8/8/8/6k1/8/4PP1P/4K2R w K - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'g');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "O-O");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CAlgebraicNotationTest, CastlingQueensideCheckmate) {
    game_t* game = simple_chess_create_game_from_fen(
        "r3k1K1/1q6/8/8/8/8/8/8 b q - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeKing, ColorBlack, 8, 'e', 8, 'c');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_LAST_MOVE_NOTATION(updated_game, "O-O-O#");

    destroy_game(game);
    destroy_game(updated_game);
}