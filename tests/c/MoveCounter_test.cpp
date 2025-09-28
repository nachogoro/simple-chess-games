#include "TestUtils.h"

TEST(CMoveCounterTest, FullMoveCounterFromStart) {
    game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 1);

    piece_move_t move1 = create_move(PieceTypePawn, ColorWhite, 2, 'e', 4, 'e');
    game_t* after_first_white_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_first_white_move);
    EXPECT_EQ(after_first_white_move->current_stage.full_moves, 1);

    piece_move_t move2 = create_move(PieceTypePawn, ColorBlack, 7, 'e', 5, 'e');
    game_t* after_black_first_move = simple_chess_make_move(after_first_white_move, move2);
    ASSERT_GAME_NOT_NULL(after_black_first_move);
    EXPECT_EQ(after_black_first_move->current_stage.full_moves, 2);

    piece_move_t move3 = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');
    game_t* after_white_response = simple_chess_make_move(after_black_first_move, move3);
    ASSERT_GAME_NOT_NULL(after_white_response);
    EXPECT_EQ(after_white_response->current_stage.full_moves, 2);

    destroy_game(game);
    destroy_game(after_first_white_move);
    destroy_game(after_black_first_move);
    destroy_game(after_white_response);
}

TEST(CMoveCounterTest, FullMoveCounterFromFenStartingWhite) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 63");
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 63);

    piece_move_t move1 = create_move(PieceTypeRook, ColorWhite, 3, 'c', 5, 'c');
    game_t* after_white_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_white_move);
    EXPECT_EQ(after_white_move->current_stage.full_moves, 63);

    piece_move_t move2 = create_move(PieceTypePawn, ColorBlack, 6, 'g', 5, 'g');
    game_t* after_black_response = simple_chess_make_move(after_white_move, move2);
    ASSERT_GAME_NOT_NULL(after_black_response);
    EXPECT_EQ(after_black_response->current_stage.full_moves, 64);

    piece_move_t move3 = create_move(PieceTypePawn, ColorWhite, 4, 'h', 5, 'g');
    game_t* after_white_next_move = simple_chess_make_move(after_black_response, move3);
    ASSERT_GAME_NOT_NULL(after_white_next_move);
    EXPECT_EQ(after_white_next_move->current_stage.full_moves, 64);

    destroy_game(game);
    destroy_game(after_white_move);
    destroy_game(after_black_response);
    destroy_game(after_white_next_move);
}

TEST(CMoveCounterTest, FullMoveCounterFromFenStartingBlack) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 b - - 0 51");
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 51);

    piece_move_t move1 = create_move(PieceTypeKnight, ColorBlack, 5, 'c', 3, 'd');
    game_t* after_black_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_black_move);
    EXPECT_EQ(after_black_move->current_stage.full_moves, 52);

    piece_move_t move2 = create_move(PieceTypeKing, ColorWhite, 2, 'b', 3, 'b');
    game_t* after_white_response = simple_chess_make_move(after_black_move, move2);
    ASSERT_GAME_NOT_NULL(after_white_response);
    EXPECT_EQ(after_white_response->current_stage.full_moves, 52);

    piece_move_t move3 = create_move(PieceTypePawn, ColorBlack, 6, 'g', 5, 'g');
    game_t* after_black_next_move = simple_chess_make_move(after_white_response, move3);
    ASSERT_GAME_NOT_NULL(after_black_next_move);
    EXPECT_EQ(after_black_next_move->current_stage.full_moves, 53);

    destroy_game(game);
    destroy_game(after_black_move);
    destroy_game(after_white_response);
    destroy_game(after_black_next_move);
}

TEST(CMoveCounterTest, HalfMoveCounter) {
    game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    // Pawn move does not increase the counter if it is 0
    piece_move_t move1 = create_move(PieceTypePawn, ColorWhite, 2, 'e', 4, 'e');
    game_t* after_white1 = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_white1);
    EXPECT_EQ(after_white1->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    // Non capture or pawn advance moves increase the counter by 1
    piece_move_t move2 = create_move(PieceTypeKnight, ColorBlack, 8, 'g', 6, 'f');
    game_t* after_black1 = simple_chess_make_move(after_white1, move2);
    ASSERT_GAME_NOT_NULL(after_black1);
    EXPECT_EQ(after_black1->current_stage.half_moves_since_last_capture_or_pawn_advance, 1);

    piece_move_t move3 = create_move(PieceTypeKnight, ColorWhite, 1, 'b', 3, 'c');
    game_t* after_white2 = simple_chess_make_move(after_black1, move3);
    ASSERT_GAME_NOT_NULL(after_white2);
    EXPECT_EQ(after_white2->current_stage.half_moves_since_last_capture_or_pawn_advance, 2);

    // Captures reset the counter to 0
    piece_move_t move4 = create_move(PieceTypeKnight, ColorBlack, 6, 'f', 4, 'e');
    game_t* after_black2 = simple_chess_make_move(after_white2, move4);
    ASSERT_GAME_NOT_NULL(after_black2);
    EXPECT_EQ(after_black2->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    piece_move_t move5 = create_move(PieceTypeQueen, ColorWhite, 1, 'd', 4, 'g');
    game_t* after_white3 = simple_chess_make_move(after_black2, move5);
    ASSERT_GAME_NOT_NULL(after_white3);
    EXPECT_EQ(after_white3->current_stage.half_moves_since_last_capture_or_pawn_advance, 1);

    piece_move_t move6 = create_move(PieceTypeKnight, ColorBlack, 8, 'b', 6, 'c');
    game_t* after_black3 = simple_chess_make_move(after_white3, move6);
    ASSERT_GAME_NOT_NULL(after_black3);
    EXPECT_EQ(after_black3->current_stage.half_moves_since_last_capture_or_pawn_advance, 2);

    destroy_game(game);
    destroy_game(after_white1);
    destroy_game(after_black1);
    destroy_game(after_white2);
    destroy_game(after_black2);
    destroy_game(after_white3);
    destroy_game(after_black3);
}