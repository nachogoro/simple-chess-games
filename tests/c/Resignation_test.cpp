#include "TestUtils.h"

TEST(CResignationTest, WhiteResignInTheirTurn) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    game_t* resigned_game = simple_chess_resign(starting_game, ColorWhite);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, GameStateBlackWon);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, WhiteResignInBlackTurn) {
    game_t* starting_game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    ASSERT_GAME_NOT_NULL(starting_game);

    game_t* resigned_game = simple_chess_resign(starting_game, ColorWhite);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, GameStateBlackWon);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, BlackResignInWhiteTurn) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    game_t* resigned_game = simple_chess_resign(starting_game, ColorBlack);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, GameStateWhiteWon);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, BlackResignInTheirTurn) {
    game_t* starting_game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    ASSERT_GAME_NOT_NULL(starting_game);

    game_t* resigned_game = simple_chess_resign(starting_game, ColorBlack);
    ASSERT_GAME_NOT_NULL(resigned_game);

    EXPECT_EQ(resigned_game->state, GameStateWhiteWon);

    destroy_game(starting_game);
    destroy_game(resigned_game);
}

TEST(CResignationTest, ResignWithNullGame) {
    game_t* resigned_game = simple_chess_resign(nullptr, ColorWhite);

    // C interface should return null when passed null game
    EXPECT_EQ(resigned_game, nullptr);
}