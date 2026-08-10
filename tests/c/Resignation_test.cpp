#include "TestUtils.h"

TEST(CResignationTest, WhiteResignInTheirTurn) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_game_t* resigned_game = simple_chess_resign(starting_game, SIMPLE_CHESS_COLOR_WHITE);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, WhiteResignInBlackTurn) {
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_game_t* resigned_game = simple_chess_resign(starting_game, SIMPLE_CHESS_COLOR_WHITE);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, BlackResignInWhiteTurn) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_game_t* resigned_game = simple_chess_resign(starting_game, SIMPLE_CHESS_COLOR_BLACK);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, SIMPLE_CHESS_GAME_STATE_WHITE_WON);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, BlackResignInTheirTurn) {
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_game_t* resigned_game = simple_chess_resign(starting_game, SIMPLE_CHESS_COLOR_BLACK);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, SIMPLE_CHESS_GAME_STATE_WHITE_WON);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, ResignWithNullGame) {
    simple_chess_game_t* resigned_game = simple_chess_resign(nullptr, SIMPLE_CHESS_COLOR_WHITE);

    // C interface should return null when passed null game
    EXPECT_EQ(resigned_game, nullptr);
}