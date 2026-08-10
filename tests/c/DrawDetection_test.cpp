#include "TestUtils.h"

namespace
{
	const simple_chess_piece_move_t whiteKnightForward = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');
	const simple_chess_piece_move_t whiteKnightBack   = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 3, 'f', 1, 'g');
	const simple_chess_piece_move_t blackKnightForward = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 8, 'g', 6, 'f');
	const simple_chess_piece_move_t blackKnightBack   = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 6, 'f', 8, 'g');

	// One round = forward+forward+back+back, returning to the starting position.
	const simple_chess_piece_move_t oneRound[] = {
		whiteKnightForward, blackKnightForward,
		whiteKnightBack,    blackKnightBack
	};

	// Play a number of full knight-shuffle rounds. Returns nullptr on failure.
	// Caller must destroy the returned game (and the input game if desired).
	simple_chess_game_t* playRounds(const simple_chess_game_t* game, int rounds)
	{
		simple_chess_game_t* current = nullptr;
		const simple_chess_game_t* prev = game;

		for (int r = 0; r < rounds; ++r)
		{
			for (int i = 0; i < 4; ++i)
			{
				current = simple_chess_make_move(prev, oneRound[i]);
				if (prev != game) simple_chess_destroy_game(const_cast<simple_chess_game_t*>(prev));
				if (!current) return nullptr;
				prev = current;
			}
		}
		return current;
	}
}

TEST(CDrawDetectionTest, OfferDraw) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    EXPECT_FALSE(is_draw_claimable(starting_game));
    EXPECT_EQ(simple_chess_game_state(updated_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(updated_game));
    EXPECT_EQ(reason_to_claim_draw(updated_game), SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated_game);
}

TEST(CDrawDetectionTest, OfferDrawAndAccept) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Accept draw
    simple_chess_game_t* drawn_game = simple_chess_claim_draw(updated_game);
    ASSERT_GAME_NOT_NULL(drawn_game);

    EXPECT_EQ(simple_chess_game_state(drawn_game), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn_game), SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated_game);
    simple_chess_destroy_game(drawn_game);
}

TEST(CDrawDetectionTest, OfferDrawAndReject) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Make a move without offering/claiming draw (effectively rejecting the draw)
    simple_chess_piece_move_t pawn_move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 7, 'a', 6, 'a');
    simple_chess_game_t* continued_game = simple_chess_make_move(updated_game, pawn_move);
    ASSERT_GAME_NOT_NULL(continued_game);

    EXPECT_EQ(simple_chess_game_state(continued_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_FALSE(is_draw_claimable(continued_game));

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated_game);
    simple_chess_destroy_game(continued_game);
}

TEST(CDrawDetectionTest, ClaimDrawWhenNotAvailable) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(starting_game);

    // Try to claim draw when no draw is available
    simple_chess_game_t* result = simple_chess_claim_draw(starting_game);

    // C interface should return null when draw claim is invalid
    EXPECT_EQ(result, nullptr);

    simple_chess_destroy_game(starting_game);
}

// ===== OpponentInsufficientMaterial tests =====

TEST(CDrawDetectionTest, OpponentInsufficientMaterialClaimable) {
    // White has K+Q, Black has only K — White can claim draw
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(game));
    EXPECT_EQ(reason_to_claim_draw(game), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_game_t* drawn = simple_chess_claim_draw(game);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, OpponentInsufficientMaterialNotClaimableWhenOpponentHasPieces) {
    // Black has K+pawn — no claim available
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/4p3/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_FALSE(is_draw_claimable(game));

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, OpponentInsufficientMaterialNeverAutomatic) {
    // Even with Automatic enforcement, should not auto-draw
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(game));
    EXPECT_EQ(reason_to_claim_draw(game), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
}

// ===== SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY tests =====

TEST(CDrawDetectionTest, ClaimOnlyInsufficientMaterialNotAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "8/3k4/8/4p3/3K4/8/8/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);

    simple_chess_piece_move_t pawn_capture = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'e');
    simple_chess_game_t* no_material = simple_chess_make_move(game, pawn_capture);
    ASSERT_GAME_NOT_NULL(no_material);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(no_material), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(no_material));
    EXPECT_EQ(reason_to_claim_draw(no_material), SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(no_material);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(no_material);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlySeventyFiveMoveRuleNotAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t king_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 2, 'd', 2, 'c');
    simple_chess_game_t* after_move = simple_chess_make_move(game, king_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(after_move), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(after_move));
    EXPECT_EQ(reason_to_claim_draw(after_move), SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(after_move);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlyFiveFoldRepetitionNotAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_new_game(SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);

    // 4 full rounds = 16 half-moves, reaching the starting position 5 times
    simple_chess_game_t* five_fold = playRounds(game, 4);
    ASSERT_GAME_NOT_NULL(five_fold);

    // With ClaimOnly, fivefold repetition should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(five_fold), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(five_fold));
    EXPECT_EQ(reason_to_claim_draw(five_fold), SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(five_fold);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(five_fold);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlyStalemateStillAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t cause_stalemate = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 5, 'h', 4, 'h');
    simple_chess_game_t* after_move = simple_chess_make_move(game, cause_stalemate);
    ASSERT_GAME_NOT_NULL(after_move);

    // Stalemate should ALWAYS be enforced, even in ClaimOnly mode
    EXPECT_EQ(simple_chess_game_state(after_move), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(after_move), SIMPLE_CHESS_DRAW_REASON_STALEMATE);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CDrawDetectionTest, ClaimOnlyCheckmateStillEnforced) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "1r3k2/8/8/8/8/8/4PPPP/6K1 b - - 149 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t checkmate_move = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 8, 'b', 1, 'b');
    simple_chess_game_t* after_move = simple_chess_make_move(game, checkmate_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Checkmate is always enforced
    EXPECT_EQ(simple_chess_game_state(after_move), SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CDrawDetectionTest, DrawEnforcementPreservedAcrossMoves) {
    simple_chess_game_t* game = simple_chess_create_new_game(SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(simple_chess_game_draw_enforcement(game), SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');
    simple_chess_game_t* after_move = simple_chess_make_move(game, knight_move);
    ASSERT_GAME_NOT_NULL(after_move);
    EXPECT_EQ(simple_chess_game_draw_enforcement(after_move), SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CDrawDetectionTest, DefaultDrawEnforcementIsAutomatic) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(simple_chess_game_draw_enforcement(game), SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, FinishedGamesReportNoDrawToClaim) {
    // Stalemate: drawn, with nothing left to claim.
    simple_chess_game_t* stalemated = simple_chess_create_game_from_fen(
            "7k/5Q2/6K1/8/8/8/8/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(stalemated);
    EXPECT_EQ(simple_chess_game_state(stalemated), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_FALSE(is_draw_claimable(stalemated));

    // Checkmate delivered on the hundredth halfmove, when the fifty-move
    // rule was available right up until the game ended.
    simple_chess_game_t* won = simple_chess_create_game_from_fen(
            "7k/5KQ1/8/8/8/8/8/8 b - - 100 100",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);
    ASSERT_GAME_NOT_NULL(won);
    EXPECT_EQ(simple_chess_game_state(won), SIMPLE_CHESS_GAME_STATE_WHITE_WON);
    EXPECT_FALSE(is_draw_claimable(won));

    simple_chess_destroy_game(stalemated);
    simple_chess_destroy_game(won);
}
