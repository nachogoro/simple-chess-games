#include "TestUtils.h"

TEST(CFenGenerationTest, BlackMoveNoCapture) {
    game_t* game = simple_chess_create_game_from_fen(
        "rnbqkbnr/ppp2ppp/8/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR b KQkq - 3 5");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeQueen, ColorBlack, 8, 'd', 6, 'f');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_STREQ(updated_game->current_stage.fen,
        "rnb1kbnr/ppp2ppp/5q2/3pp1B1/4P3/3P4/PPP2PPP/RN1QKBNR w KQkq - 4 6");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CFenGenerationTest, BlackMoveWithCapture) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/pB1K4/7N/8/1RnP1P1q/4P3/4k3/8 b - - 15 26");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_move(PieceTypeQueen, ColorBlack, 4, 'h', 6, 'h');
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_STREQ(updated_game->current_stage.fen,
        "8/pB1K4/7q/8/1RnP1P2/4P3/4k3/8 w - - 0 27");

    destroy_game(game);
    destroy_game(updated_game);
}

TEST(CFenGenerationTest, BlackPawnMove) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/1B1K4/7N/8/1RnP1P1q/4P3/p3k3/8 b - - 15 26");
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t move = create_promotion_move(ColorBlack, 2, 'a', 1, 'a', PieceTypeQueen);
    game_t* updated_game = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_STREQ(updated_game->current_stage.fen,
        "8/1B1K4/7N/8/1RnP1P1q/4P3/4k3/q7 w - - 0 27");

    destroy_game(game);
    destroy_game(updated_game);
}