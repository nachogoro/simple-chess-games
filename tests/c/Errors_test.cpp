#include "TestUtils.h"

// Every failure used to be a NULL return and nothing else, so these check
// that each one is now told apart from the others.

TEST(CErrorsTest, InvalidFen) {
    simple_chess_error_t error = SIMPLE_CHESS_OK;
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "not a position",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC,
            &error);

    EXPECT_EQ(game, nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_INVALID_FEN);
}

TEST(CErrorsTest, NullFen) {
    simple_chess_error_t error = SIMPLE_CHESS_OK;
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            nullptr, SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, &error);

    EXPECT_EQ(game, nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
}

TEST(CErrorsTest, IllegalMoveInAPlayableGame) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, nullptr);
    ASSERT_GAME_NOT_NULL(game);

    // A pawn cannot advance three squares.
    const simple_chess_piece_move_t move = create_move(
            SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE,
            2, 'e', 5, 'e');

    simple_chess_error_t error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_make_move(game, move, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_ILLEGAL_MOVE);

    simple_chess_destroy_game(game);
}

TEST(CErrorsTest, MoveInAFinishedGame) {
    // Fool's mate: black has already won.
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC,
            nullptr);
    ASSERT_GAME_NOT_NULL(game);
    ASSERT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    const simple_chess_piece_move_t move = create_move(
            SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE,
            2, 'a', 3, 'a');

    simple_chess_error_t error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_make_move(game, move, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_GAME_OVER);

    error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_resign(game, SIMPLE_CHESS_COLOR_WHITE, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_GAME_OVER);

    error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_claim_draw(game, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_GAME_OVER);

    simple_chess_destroy_game(game);
}

TEST(CErrorsTest, DrawNotClaimable) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, nullptr);
    ASSERT_GAME_NOT_NULL(game);
    ASSERT_FALSE(is_draw_claimable(game));

    simple_chess_error_t error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_claim_draw(game, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_DRAW_NOT_CLAIMABLE);

    simple_chess_destroy_game(game);
}

TEST(CErrorsTest, NullGame) {
    simple_chess_error_t error = SIMPLE_CHESS_OK;
    EXPECT_EQ(simple_chess_copy_game(nullptr, &error), nullptr);
    EXPECT_EQ(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
}

TEST(CErrorsTest, SuccessReportsOk) {
    simple_chess_error_t error = SIMPLE_CHESS_ERROR_OUT_OF_MEMORY;
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, &error);

    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(error, SIMPLE_CHESS_OK);

    simple_chess_destroy_game(game);
}

TEST(CErrorsTest, ErrorPointerIsOptional) {
    // Passing NULL for the error is allowed and must not crash, whether the
    // call succeeds or fails.
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, nullptr);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_claim_draw(game, nullptr), nullptr);
    EXPECT_EQ(
            simple_chess_create_game_from_fen(
                "nonsense", SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, nullptr),
            nullptr);

    simple_chess_destroy_game(game);
}

TEST(CErrorsTest, EveryErrorHasADescription) {
    const simple_chess_error_t all[] = {
        SIMPLE_CHESS_OK,
        SIMPLE_CHESS_ERROR_NULL_ARGUMENT,
        SIMPLE_CHESS_ERROR_INVALID_FEN,
        SIMPLE_CHESS_ERROR_ILLEGAL_MOVE,
        SIMPLE_CHESS_ERROR_GAME_OVER,
        SIMPLE_CHESS_ERROR_DRAW_NOT_CLAIMABLE,
        SIMPLE_CHESS_ERROR_OUT_OF_MEMORY,
    };

    for (const simple_chess_error_t error : all) {
        const char* description = simple_chess_error_string(error);
        ASSERT_NE(description, nullptr);
        EXPECT_GT(strlen(description), 0u);
    }
}
