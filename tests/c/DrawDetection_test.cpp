#include "TestUtils.h"

TEST(CDrawDetectionTest, OfferDraw) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    piece_move_t knight_move = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');

    // Offer draw
    game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_FALSE(starting_game->is_draw_claimable);
    EXPECT_EQ(updated_game->state, GameStatePlaying);
    EXPECT_TRUE(updated_game->is_draw_claimable);
    EXPECT_EQ(updated_game->reason_to_claim_draw, DrawReasonOfferedAndAccepted);

    destroy_game(starting_game);
    destroy_game(updated_game);
}

TEST(CDrawDetectionTest, OfferDrawAndAccept) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    piece_move_t knight_move = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');

    // Offer draw
    game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Accept draw
    game_t* drawn_game = simple_chess_claim_draw(updated_game);
    ASSERT_GAME_NOT_NULL(drawn_game);

    EXPECT_EQ(drawn_game->state, GameStateDrawn);
    EXPECT_EQ(drawn_game->draw_reason, DrawReasonOfferedAndAccepted);

    destroy_game(starting_game);
    destroy_game(updated_game);
    destroy_game(drawn_game);
}

TEST(CDrawDetectionTest, OfferDrawAndReject) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    piece_move_t knight_move = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');

    // Offer draw
    game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Make a move without offering/claiming draw (effectively rejecting the draw)
    piece_move_t pawn_move = create_move(PieceTypePawn, ColorBlack, 7, 'a', 6, 'a');
    game_t* continued_game = simple_chess_make_move(updated_game, pawn_move);
    ASSERT_GAME_NOT_NULL(continued_game);

    EXPECT_EQ(continued_game->state, GameStatePlaying);
    EXPECT_FALSE(continued_game->is_draw_claimable);

    destroy_game(starting_game);
    destroy_game(updated_game);
    destroy_game(continued_game);
}

TEST(CDrawDetectionTest, ClaimDrawWhenNotAvailable) {
    game_t* starting_game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(starting_game);

    // Try to claim draw when no draw is available
    game_t* result = simple_chess_claim_draw(starting_game);

    // C interface should return null when draw claim is invalid
    EXPECT_EQ(result, nullptr);

    destroy_game(starting_game);
}