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
				current = simple_chess_make_move(prev, oneRound[i], NULL);
				if (prev != game) simple_chess_destroy_game(const_cast<simple_chess_game_t*>(prev));
				if (!current) return nullptr;
				prev = current;
			}
		}
		return current;
	}

	// Play one move without taking ownership of anything: the caller destroys
	// both the game passed in and the one returned.
	simple_chess_game_t* play(
			const simple_chess_game_t* game,
			const simple_chess_piece_move_t& move)
	{
		return simple_chess_make_move(game, move, NULL);
	}

	// The two questions a position answers on its own, with no moves played
	simple_chess_game_state_t state_of(const char* fen)
	{
		simple_chess_game_t* game = simple_chess_create_game_from_fen(
				fen, SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
		EXPECT_GAME_NOT_NULL(game);
		const simple_chess_game_state_t state = simple_chess_game_state(game);
		simple_chess_destroy_game(game);
		return state;
	}

	bool draw_claimable_in(const char* fen)
	{
		simple_chess_game_t* game = simple_chess_create_game_from_fen(
				fen, SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
		EXPECT_GAME_NOT_NULL(game);
		const bool claimable = is_draw_claimable(game);
		simple_chess_destroy_game(game);
		return claimable;
	}
}

TEST(CDrawDetectionTest, OfferDraw) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true, NULL);
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
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Accept draw
    simple_chess_game_t* drawn_game = simple_chess_claim_draw(updated_game, NULL);
    ASSERT_GAME_NOT_NULL(drawn_game);

    EXPECT_EQ(simple_chess_game_state(drawn_game), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn_game), SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated_game);
    simple_chess_destroy_game(drawn_game);
}

TEST(CDrawDetectionTest, OfferDrawAndReject) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');

    // Offer draw
    simple_chess_game_t* updated_game = simple_chess_make_move_with_draw_offer(starting_game, knight_move, true, NULL);
    ASSERT_GAME_NOT_NULL(updated_game);

    // Make a move without offering/claiming draw (effectively rejecting the draw)
    simple_chess_piece_move_t pawn_move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 7, 'a', 6, 'a');
    simple_chess_game_t* continued_game = simple_chess_make_move(updated_game, pawn_move, NULL);
    ASSERT_GAME_NOT_NULL(continued_game);

    EXPECT_EQ(simple_chess_game_state(continued_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_FALSE(is_draw_claimable(continued_game));

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated_game);
    simple_chess_destroy_game(continued_game);
}

TEST(CDrawDetectionTest, Stalemate) {
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
            "8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);
    EXPECT_FALSE(is_draw_claimable(starting_game));

    const simple_chess_piece_move_t cause_stalemate = create_move(
            SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 5, 'h', 4, 'h');
    simple_chess_game_t* updated = play(starting_game, cause_stalemate);
    ASSERT_GAME_NOT_NULL(updated);

    EXPECT_EQ(simple_chess_game_state(updated), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(updated), SIMPLE_CHESS_DRAW_REASON_STALEMATE);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(updated);
}

// ===== InsufficientMaterial tests =====

namespace
{
    // The capture that strips the board down to the material under test
    void expect_insufficient_material_after(
            const char* fen,
            simple_chess_piece_type_t piece_type, simple_chess_color_t color,
            uint8_t src_rank, char src_file, uint8_t dst_rank, char dst_file,
            simple_chess_game_state_t expected_state)
    {
        simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
                fen, SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
        ASSERT_GAME_NOT_NULL(starting_game);
        EXPECT_EQ(simple_chess_game_state(starting_game), SIMPLE_CHESS_GAME_STATE_PLAYING);

        const simple_chess_piece_move_t capture = create_move(
                piece_type, color, src_rank, src_file, dst_rank, dst_file);
        simple_chess_game_t* after_capture = play(starting_game, capture);
        ASSERT_GAME_NOT_NULL(after_capture);

        EXPECT_EQ(simple_chess_game_state(after_capture), expected_state);
        if (expected_state == SIMPLE_CHESS_GAME_STATE_DRAWN)
        {
            EXPECT_EQ(draw_reason(after_capture),
                    SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL);
        }

        simple_chess_destroy_game(starting_game);
        simple_chess_destroy_game(after_capture);
    }
}

TEST(CDrawDetectionTest, InsufficientMaterialKingvsKing) {
    expect_insufficient_material_after(
            "8/3k4/8/4p3/3K4/8/8/8 w - - 0 1",
            SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'e',
            SIMPLE_CHESS_GAME_STATE_DRAWN);
}

TEST(CDrawDetectionTest, InsufficientMaterialKingvsKingAndBishop) {
    expect_insufficient_material_after(
            "3k4/4R3/2B5/8/3K4/8/8/8 b - - 0 1",
            SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'd', 7, 'e',
            SIMPLE_CHESS_GAME_STATE_DRAWN);
}

TEST(CDrawDetectionTest, InsufficientMaterialKingvsKingAndKnight) {
    expect_insufficient_material_after(
            "3k4/3R4/2N5/8/3K4/8/8/8 b - - 0 1",
            SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'd', 7, 'd',
            SIMPLE_CHESS_GAME_STATE_DRAWN);
}

TEST(CDrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndSameColoredBishop) {
    // Bishops on g1 and c7 are both on dark squares, so neither can ever
    // attack the other's king
    expect_insufficient_material_after(
            "3k4/2b5/8/3r4/3K4/8/8/6B1 w - - 0 1",
            SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'd',
            SIMPLE_CHESS_GAME_STATE_DRAWN);
}

TEST(CDrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndOppositeColorBishop) {
    // The same position with the bishop on h1 instead: opposite colours, so
    // a mate remains possible and the game goes on
    expect_insufficient_material_after(
            "3k4/2b5/8/3r4/3K4/8/8/7B w - - 0 1",
            SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'd',
            SIMPLE_CHESS_GAME_STATE_PLAYING);
}

// ===== OpponentInsufficientMaterial tests =====

TEST(CDrawDetectionTest, OpponentInsufficientMaterialKingvsKingAndQueen) {
    // White has K+Q, Black has only K — White can claim draw
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(game));
    EXPECT_EQ(reason_to_claim_draw(game), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_game_t* drawn = simple_chess_claim_draw(game, NULL);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, OpponentInsufficientMaterialKingvsKingAndRook) {
    // White has K+R, Black has only K
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/8/8/8/8/8/3K4/3R4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(game));
    EXPECT_EQ(reason_to_claim_draw(game), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, OpponentInsufficientMaterialNotClaimableWhenOpponentHasPieces) {
    // Black has K+pawn — no claim available
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/4p3/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_FALSE(is_draw_claimable(game));

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, OpponentInsufficientMaterialNeverAutomatic) {
    // Even with Automatic enforcement, should not auto-draw
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(game));
    EXPECT_EQ(reason_to_claim_draw(game), SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL);

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, InvalidClaim) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    // Try to claim draw when no draw is available
    simple_chess_game_t* result = simple_chess_claim_draw(starting_game, NULL);

    // C interface should return null when draw claim is invalid
    EXPECT_EQ(result, nullptr);

    simple_chess_destroy_game(starting_game);
}

TEST(CDrawDetectionTest, NFoldRepetition) {
    simple_chess_game_t* starting_game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    simple_chess_game_t* move1 = play(starting_game, whiteKnightForward);
    ASSERT_GAME_NOT_NULL(move1);
    EXPECT_FALSE(is_draw_claimable(move1));
    simple_chess_game_t* move1Black = play(move1, blackKnightForward);
    ASSERT_GAME_NOT_NULL(move1Black);
    EXPECT_FALSE(is_draw_claimable(move1Black));
    simple_chess_game_t* move2 = play(move1Black, whiteKnightBack);
    ASSERT_GAME_NOT_NULL(move2);
    EXPECT_FALSE(is_draw_claimable(move2));
    simple_chess_game_t* move2Black = play(move2, blackKnightBack);
    ASSERT_GAME_NOT_NULL(move2Black);
    EXPECT_FALSE(is_draw_claimable(move2Black));

    simple_chess_game_t* move3 = play(move2Black, whiteKnightForward);
    ASSERT_GAME_NOT_NULL(move3);
    EXPECT_FALSE(is_draw_claimable(move3));
    simple_chess_game_t* move3Black = play(move3, blackKnightForward);
    ASSERT_GAME_NOT_NULL(move3Black);
    EXPECT_FALSE(is_draw_claimable(move3Black));
    simple_chess_game_t* move4 = play(move3Black, whiteKnightBack);
    ASSERT_GAME_NOT_NULL(move4);

    // At this point, black could play blackKnightBack, which would cause
    // 3-fold repetition. Therefore, black can claim a draw at this point, even
    // before making the move.
    EXPECT_TRUE(is_draw_claimable(move4));
    EXPECT_EQ(reason_to_claim_draw(move4), SIMPLE_CHESS_DRAW_REASON_THREEFOLD_REPETITION);

    // Complete the remaining rounds to reach 5-fold repetition.
    // move4 is mid-cycle (white just moved back), so we finish
    // this round and play 2 more full rounds from the reset position.
    simple_chess_game_t* endOfRound2 = play(move4, blackKnightBack);
    ASSERT_GAME_NOT_NULL(endOfRound2);
    simple_chess_game_t* fiveFold = playRounds(endOfRound2, 2);
    ASSERT_GAME_NOT_NULL(fiveFold);

    EXPECT_EQ(simple_chess_game_state(fiveFold), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(fiveFold), SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(move1);
    simple_chess_destroy_game(move1Black);
    simple_chess_destroy_game(move2);
    simple_chess_destroy_game(move2Black);
    simple_chess_destroy_game(move3);
    simple_chess_destroy_game(move3Black);
    simple_chess_destroy_game(move4);
    simple_chess_destroy_game(endOfRound2);
    simple_chess_destroy_game(fiveFold);
}

TEST(CDrawDetectionTest, FiftyMoveRule) {
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
            "3k4/2b5/8/3r4/8/8/3K4/7B w - - 98 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    EXPECT_EQ(simple_chess_game_state(starting_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_FALSE(is_draw_claimable(starting_game));

    // Available when the move would be the 50th
    simple_chess_game_t* one_before_fifty = play(starting_game, create_move(
                SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 2, 'd', 2, 'c'));
    ASSERT_GAME_NOT_NULL(one_before_fifty);
    EXPECT_EQ(simple_chess_game_state(one_before_fifty), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(one_before_fifty));
    EXPECT_EQ(reason_to_claim_draw(one_before_fifty), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    // Not automatically claimed at fifty
    simple_chess_game_t* exactly_fifty = play(one_before_fifty, create_move(
                SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_BLACK, 7, 'c', 6, 'b'));
    ASSERT_GAME_NOT_NULL(exactly_fifty);
    EXPECT_EQ(simple_chess_game_state(exactly_fifty), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(exactly_fifty));
    EXPECT_EQ(reason_to_claim_draw(exactly_fifty), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    // Nor after it
    simple_chess_game_t* after_fifty = play(exactly_fifty, create_move(
                SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 1, 'h', 2, 'g'));
    ASSERT_GAME_NOT_NULL(after_fifty);
    EXPECT_EQ(simple_chess_game_state(after_fifty), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(after_fifty));
    EXPECT_EQ(reason_to_claim_draw(after_fifty), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(one_before_fifty);
    simple_chess_destroy_game(exactly_fifty);
    simple_chess_destroy_game(after_fifty);
}

TEST(CDrawDetectionTest, SeventyFiveMoveRule) {
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
            "3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    EXPECT_EQ(simple_chess_game_state(starting_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(starting_game));
    EXPECT_EQ(reason_to_claim_draw(starting_game), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    simple_chess_game_t* seventy_five_full_moves = play(starting_game, create_move(
                SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 2, 'd', 2, 'c'));
    ASSERT_GAME_NOT_NULL(seventy_five_full_moves);

    EXPECT_EQ(simple_chess_game_state(seventy_five_full_moves), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(seventy_five_full_moves), SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(seventy_five_full_moves);
}

TEST(CDrawDetectionTest, SeventyFiveMoveRuleCheckmateOverride) {
    // Position where black can deliver checkmate on the 75th move
    simple_chess_game_t* starting_game = simple_chess_create_game_from_fen(
            "1r3k2/8/8/8/8/8/4PPPP/6K1 b - - 149 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(starting_game);

    EXPECT_EQ(simple_chess_game_state(starting_game), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(starting_game));
    EXPECT_EQ(reason_to_claim_draw(starting_game), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    simple_chess_game_t* checkmate_on_75th = play(starting_game, create_move(
                SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 8, 'b', 1, 'b'));
    ASSERT_GAME_NOT_NULL(checkmate_on_75th);

    // Checkmate takes precedence over the 75-move rule
    EXPECT_EQ(simple_chess_game_state(checkmate_on_75th), SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    simple_chess_destroy_game(starting_game);
    simple_chess_destroy_game(checkmate_on_75th);
}

// ===== SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY tests =====

TEST(CDrawDetectionTest, ClaimOnlyInsufficientMaterialNotAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "8/3k4/8/4p3/3K4/8/8/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_state(game), SIMPLE_CHESS_GAME_STATE_PLAYING);

    simple_chess_piece_move_t pawn_capture = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'e');
    simple_chess_game_t* no_material = simple_chess_make_move(game, pawn_capture, NULL);
    ASSERT_GAME_NOT_NULL(no_material);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(no_material), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(no_material));
    EXPECT_EQ(reason_to_claim_draw(no_material), SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(no_material, NULL);
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
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t king_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 2, 'd', 2, 'c');
    simple_chess_game_t* after_move = simple_chess_make_move(game, king_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // With ClaimOnly, should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(after_move), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(after_move));
    EXPECT_EQ(reason_to_claim_draw(after_move), SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(after_move, NULL);
    ASSERT_GAME_NOT_NULL(drawn);
    EXPECT_EQ(simple_chess_game_state(drawn), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(draw_reason(drawn), SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
    simple_chess_destroy_game(drawn);
}

TEST(CDrawDetectionTest, ClaimOnlyFiveFoldRepetitionNotAutoDrawn) {
    simple_chess_game_t* game = simple_chess_create_new_game(SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // 4 full rounds = 16 half-moves, reaching the starting position 5 times
    simple_chess_game_t* five_fold = playRounds(game, 4);
    ASSERT_GAME_NOT_NULL(five_fold);

    // With ClaimOnly, fivefold repetition should NOT auto-draw
    EXPECT_EQ(simple_chess_game_state(five_fold), SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_TRUE(is_draw_claimable(five_fold));
    EXPECT_EQ(reason_to_claim_draw(five_fold), SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION);

    // But it should still be claimable
    simple_chess_game_t* drawn = simple_chess_claim_draw(five_fold, NULL);
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
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t cause_stalemate = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 5, 'h', 4, 'h');
    simple_chess_game_t* after_move = simple_chess_make_move(game, cause_stalemate, NULL);
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
            SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t checkmate_move = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK, 8, 'b', 1, 'b');
    simple_chess_game_t* after_move = simple_chess_make_move(game, checkmate_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Checkmate is always enforced
    EXPECT_EQ(simple_chess_game_state(after_move), SIMPLE_CHESS_GAME_STATE_BLACK_WON);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CDrawDetectionTest, DrawEnforcementPreservedAcrossMoves) {
    simple_chess_game_t* game = simple_chess_create_new_game(SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY, NULL);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(simple_chess_game_draw_enforcement(game), SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);

    simple_chess_piece_move_t knight_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');
    simple_chess_game_t* after_move = simple_chess_make_move(game, knight_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);
    EXPECT_EQ(simple_chess_game_draw_enforcement(after_move), SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY);

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CDrawDetectionTest, DefaultDrawEnforcementIsAutomatic) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);
    EXPECT_EQ(simple_chess_game_draw_enforcement(game), SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC);

    simple_chess_destroy_game(game);
}

TEST(CDrawDetectionTest, NoDrawToClaimInAStalematedGame) {
    const char* stalemate = "7k/5Q2/6K1/8/8/8/8/8 b - - 0 1";

    EXPECT_EQ(state_of(stalemate), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_FALSE(draw_claimable_in(stalemate));
}

TEST(CDrawDetectionTest, NoDrawToClaimWithInsufficientMaterial) {
    const char* lone_kings = "7k/8/6K1/8/8/8/8/8 w - - 0 1";

    EXPECT_EQ(state_of(lone_kings), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_FALSE(draw_claimable_in(lone_kings));
}

TEST(CDrawDetectionTest, NoDrawToClaimInAGameSomebodyWon) {
    // Checkmate delivered on the hundredth halfmove since the last capture
    // or pawn move, so the fifty-move rule was available right up until the
    // game ended. The win takes precedence and leaves nothing to claim.
    const char* mate_on_the_fiftieth_move = "7k/5KQ1/8/8/8/8/8/8 b - - 100 100";

    EXPECT_EQ(state_of(mate_on_the_fiftieth_move), SIMPLE_CHESS_GAME_STATE_WHITE_WON);
    EXPECT_FALSE(draw_claimable_in(mate_on_the_fiftieth_move));
}

// The queries above no longer throw for a game which has ended, so what they
// report about one is now something a caller can see.

TEST(CDrawDetectionTest, FinishedGamesReportNoDrawToClaim) {
    // Stalemate: drawn, with nothing left to claim.
    simple_chess_game_t* stalemated = simple_chess_create_game_from_fen(
            "7k/5Q2/6K1/8/8/8/8/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(stalemated);
    EXPECT_EQ(simple_chess_game_state(stalemated), SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_FALSE(is_draw_claimable(stalemated));

    // Checkmate delivered on the hundredth halfmove, when the fifty-move
    // rule was available right up until the game ended.
    simple_chess_game_t* won = simple_chess_create_game_from_fen(
            "7k/5KQ1/8/8/8/8/8/8 b - - 100 100",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(won);
    EXPECT_EQ(simple_chess_game_state(won), SIMPLE_CHESS_GAME_STATE_WHITE_WON);
    EXPECT_FALSE(is_draw_claimable(won));

    // A drawn game says why it was drawn, and offers nothing to claim.
    EXPECT_EQ(draw_reason(stalemated), SIMPLE_CHESS_DRAW_REASON_STALEMATE);

    // Resigning ends a game in which a draw was claimable, and the offer
    // goes with it.
    simple_chess_game_t* claimable = simple_chess_create_game_from_fen(
            "7k/7r/8/8/8/8/R7/K7 w - - 100 100",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(claimable);
    ASSERT_EQ(simple_chess_game_state(claimable), SIMPLE_CHESS_GAME_STATE_PLAYING);
    ASSERT_TRUE(is_draw_claimable(claimable));
    ASSERT_EQ(reason_to_claim_draw(claimable), SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE);

    simple_chess_game_t* resigned = simple_chess_resign(
            claimable, SIMPLE_CHESS_COLOR_WHITE, NULL);
    ASSERT_GAME_NOT_NULL(resigned);
    ASSERT_EQ(simple_chess_game_state(resigned), SIMPLE_CHESS_GAME_STATE_BLACK_WON);
    EXPECT_FALSE(is_draw_claimable(resigned));

    simple_chess_destroy_game(stalemated);
    simple_chess_destroy_game(won);
    simple_chess_destroy_game(claimable);
    simple_chess_destroy_game(resigned);
}
