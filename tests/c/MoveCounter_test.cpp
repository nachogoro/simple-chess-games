#include "TestUtils.h"

TEST(CMoveCounterTest, FullMoveCounterFromStart) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 1);

    simple_chess_piece_move_t move1 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'e', 4, 'e');
    simple_chess_game_t* after_first_white_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_first_white_move);
    EXPECT_EQ(after_first_white_move->current_stage.full_moves, 1);

    simple_chess_piece_move_t move2 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 7, 'e', 5, 'e');
    simple_chess_game_t* after_black_first_move = simple_chess_make_move(after_first_white_move, move2);
    ASSERT_GAME_NOT_NULL(after_black_first_move);
    EXPECT_EQ(after_black_first_move->current_stage.full_moves, 2);

    simple_chess_piece_move_t move3 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');
    simple_chess_game_t* after_white_response = simple_chess_make_move(after_black_first_move, move3);
    ASSERT_GAME_NOT_NULL(after_white_response);
    EXPECT_EQ(after_white_response->current_stage.full_moves, 2);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_first_white_move);
    simple_chess_destroy_game(after_black_first_move);
    simple_chess_destroy_game(after_white_response);
}

TEST(CMoveCounterTest, FullMoveCounterFromFenStartingWhite) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 63",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 63);

    simple_chess_piece_move_t move1 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 3, 'c', 5, 'c');
    simple_chess_game_t* after_white_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_white_move);
    EXPECT_EQ(after_white_move->current_stage.full_moves, 63);

    simple_chess_piece_move_t move2 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 6, 'g', 5, 'g');
    simple_chess_game_t* after_black_response = simple_chess_make_move(after_white_move, move2);
    ASSERT_GAME_NOT_NULL(after_black_response);
    EXPECT_EQ(after_black_response->current_stage.full_moves, 64);

    simple_chess_piece_move_t move3 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 4, 'h', 5, 'g');
    simple_chess_game_t* after_white_next_move = simple_chess_make_move(after_black_response, move3);
    ASSERT_GAME_NOT_NULL(after_white_next_move);
    EXPECT_EQ(after_white_next_move->current_stage.full_moves, 64);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_white_move);
    simple_chess_destroy_game(after_black_response);
    simple_chess_destroy_game(after_white_next_move);
}

TEST(CMoveCounterTest, FullMoveCounterFromFenStartingBlack) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 b - - 0 51",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.full_moves, 51);

    simple_chess_piece_move_t move1 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 5, 'c', 3, 'd');
    simple_chess_game_t* after_black_move = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_black_move);
    EXPECT_EQ(after_black_move->current_stage.full_moves, 52);

    simple_chess_piece_move_t move2 = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 2, 'b', 3, 'b');
    simple_chess_game_t* after_white_response = simple_chess_make_move(after_black_move, move2);
    ASSERT_GAME_NOT_NULL(after_white_response);
    EXPECT_EQ(after_white_response->current_stage.full_moves, 52);

    simple_chess_piece_move_t move3 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 6, 'g', 5, 'g');
    simple_chess_game_t* after_black_next_move = simple_chess_make_move(after_white_response, move3);
    ASSERT_GAME_NOT_NULL(after_black_next_move);
    EXPECT_EQ(after_black_next_move->current_stage.full_moves, 53);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_black_move);
    simple_chess_destroy_game(after_white_response);
    simple_chess_destroy_game(after_black_next_move);
}

TEST(CMoveCounterTest, HalfMoveCounter) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    // Pawn move does not increase the counter if it is 0
    simple_chess_piece_move_t move1 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'e', 4, 'e');
    simple_chess_game_t* after_white1 = simple_chess_make_move(game, move1);
    ASSERT_GAME_NOT_NULL(after_white1);
    EXPECT_EQ(after_white1->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    // Non capture or pawn advance moves increase the counter by 1
    simple_chess_piece_move_t move2 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 8, 'g', 6, 'f');
    simple_chess_game_t* after_black1 = simple_chess_make_move(after_white1, move2);
    ASSERT_GAME_NOT_NULL(after_black1);
    EXPECT_EQ(after_black1->current_stage.half_moves_since_last_capture_or_pawn_advance, 1);

    simple_chess_piece_move_t move3 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'b', 3, 'c');
    simple_chess_game_t* after_white2 = simple_chess_make_move(after_black1, move3);
    ASSERT_GAME_NOT_NULL(after_white2);
    EXPECT_EQ(after_white2->current_stage.half_moves_since_last_capture_or_pawn_advance, 2);

    // Captures reset the counter to 0
    simple_chess_piece_move_t move4 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 6, 'f', 4, 'e');
    simple_chess_game_t* after_black2 = simple_chess_make_move(after_white2, move4);
    ASSERT_GAME_NOT_NULL(after_black2);
    EXPECT_EQ(after_black2->current_stage.half_moves_since_last_capture_or_pawn_advance, 0);

    simple_chess_piece_move_t move5 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 1, 'd', 4, 'g');
    simple_chess_game_t* after_white3 = simple_chess_make_move(after_black2, move5);
    ASSERT_GAME_NOT_NULL(after_white3);
    EXPECT_EQ(after_white3->current_stage.half_moves_since_last_capture_or_pawn_advance, 1);

    simple_chess_piece_move_t move6 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 8, 'b', 6, 'c');
    simple_chess_game_t* after_black3 = simple_chess_make_move(after_white3, move6);
    ASSERT_GAME_NOT_NULL(after_black3);
    EXPECT_EQ(after_black3->current_stage.half_moves_since_last_capture_or_pawn_advance, 2);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_white1);
    simple_chess_destroy_game(after_black1);
    simple_chess_destroy_game(after_white2);
    simple_chess_destroy_game(after_black2);
    simple_chess_destroy_game(after_white3);
    simple_chess_destroy_game(after_black3);
}