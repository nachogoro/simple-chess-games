#include "TestUtils.h"

namespace
{
	const piece_move_t whiteKnightForward = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');
	const piece_move_t whiteKnightBack   = create_move(PieceTypeKnight, ColorWhite, 3, 'f', 1, 'g');
	const piece_move_t blackKnightForward = create_move(PieceTypeKnight, ColorBlack, 8, 'g', 6, 'f');
	const piece_move_t blackKnightBack   = create_move(PieceTypeKnight, ColorBlack, 6, 'f', 8, 'g');

	// One round = forward+forward+back+back, returning to the starting position.
	const piece_move_t oneRound[] = {
		whiteKnightForward, blackKnightForward,
		whiteKnightBack,    blackKnightBack
	};

	// Play a number of full knight-shuffle rounds. Returns nullptr on failure.
	// Caller must destroy the returned game (and the input game if desired).
	game_t* playRounds(const game_t* game, int rounds)
	{
		game_t* current = nullptr;
		const game_t* prev = game;

		for (int r = 0; r < rounds; ++r)
		{
			for (int i = 0; i < 4; ++i)
			{
				current = simple_chess_make_move(prev, oneRound[i]);
				if (prev != game) destroy_game(const_cast<game_t*>(prev));
				if (!current) return nullptr;
				prev = current;
			}
		}
		return current;
	}
}

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

// ===== DrawEnforcementClaimOnly tests =====

TEST(CDrawDetectionTest, ClaimOnlyInsufficientMaterialNotAutoDrawn) {
    game_t* game = simple_chess_create_game_from_fen_ex(
            "8/3k4/8/4p3/3K4/8/8/8 w - - 0 1",
            DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, GameStatePlaying);

    piece_move_t pawn_capture = create_move(PieceTypeKing, ColorWhite, 4, 'd', 5, 'e');
    game_t* no_material = simple_chess_make_move(game, pawn_capture);
    ASSERT_GAME_NOT_NULL(no_material);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(no_material->state, GameStatePlaying);
    EXPECT_TRUE(no_material->is_draw_claimable);
    EXPECT_EQ(no_material->reason_to_claim_draw, DrawReasonInsufficientMaterial);

    // But it should still be claimable
    game_t* drawn = simple_chess_claim_draw(no_material);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(drawn->state, GameStateDrawn);
    EXPECT_EQ(drawn->draw_reason, DrawReasonInsufficientMaterial);

    destroy_game(game);
    destroy_game(no_material);
    destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlySeventyFiveMoveRuleNotAutoDrawn) {
    game_t* game = simple_chess_create_game_from_fen_ex(
            "3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1",
            DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t king_move = create_move(PieceTypeKing, ColorWhite, 2, 'd', 2, 'c');
    game_t* after_move = simple_chess_make_move(game, king_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(after_move->state, GameStatePlaying);
    EXPECT_TRUE(after_move->is_draw_claimable);
    EXPECT_EQ(after_move->reason_to_claim_draw, DrawReasonSeventyFiveMoveRule);

    // But it should still be claimable
    game_t* drawn = simple_chess_claim_draw(after_move);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(drawn->state, GameStateDrawn);
    EXPECT_EQ(drawn->draw_reason, DrawReasonSeventyFiveMoveRule);

    destroy_game(game);
    destroy_game(after_move);
    destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlyFiveFoldRepetitionNotAutoDrawn) {
    game_t* game = simple_chess_create_new_game_ex(DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);

    // 4 full rounds = 16 half-moves, reaching the starting position 5 times
    game_t* five_fold = playRounds(game, 4);
    ASSERT_GAME_NOT_NULL(five_fold);

    // With ClaimOnly, fivefold repetition should NOT auto-draw
    EXPECT_EQ(five_fold->state, GameStatePlaying);
    EXPECT_TRUE(five_fold->is_draw_claimable);
    EXPECT_EQ(five_fold->reason_to_claim_draw, DrawReasonFiveFoldRepetition);

    // But it should still be claimable
    game_t* drawn = simple_chess_claim_draw(five_fold);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(drawn->state, GameStateDrawn);
    EXPECT_EQ(drawn->draw_reason, DrawReasonFiveFoldRepetition);

    destroy_game(game);
    destroy_game(five_fold);
    destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlyStalemateStillAutoDrawn) {
    game_t* game = simple_chess_create_game_from_fen_ex(
            "8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1",
            DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t cause_stalemate = create_move(PieceTypeRook, ColorBlack, 5, 'h', 4, 'h');
    game_t* after_move = simple_chess_make_move(game, cause_stalemate);
    ASSERT_GAME_NOT_NULL(after_move);

    // Stalemate should ALWAYS be enforced, even in ClaimOnly mode
    EXPECT_EQ(after_move->state, GameStateDrawn);
    EXPECT_EQ(after_move->draw_reason, DrawReasonStaleMate);

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CDrawDetectionTest, ClaimOnlyCheckmateStillEnforced) {
    game_t* game = simple_chess_create_game_from_fen_ex(
            "1r3k2/8/8/8/8/8/4PPPP/6K1 b - - 149 1",
            DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);

    piece_move_t checkmate_move = create_move(PieceTypeRook, ColorBlack, 8, 'b', 1, 'b');
    game_t* after_move = simple_chess_make_move(game, checkmate_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Checkmate is always enforced
    EXPECT_EQ(after_move->state, GameStateBlackWon);

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CDrawDetectionTest, DrawEnforcementPreservedAcrossMoves) {
    game_t* game = simple_chess_create_new_game_ex(DrawEnforcementClaimOnly);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->draw_enforcement, DrawEnforcementClaimOnly);

    piece_move_t knight_move = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');
    game_t* after_move = simple_chess_make_move(game, knight_move);
    ASSERT_GAME_NOT_NULL(after_move);
    EXPECT_EQ(after_move->draw_enforcement, DrawEnforcementClaimOnly);

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CDrawDetectionTest, DefaultDrawEnforcementIsAutomatic) {
    game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(game->draw_enforcement, DrawEnforcementAutomatic);

    destroy_game(game);
}
