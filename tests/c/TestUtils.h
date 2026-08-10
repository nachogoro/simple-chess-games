#ifndef TEST_UTILS_C_H
#define TEST_UTILS_C_H

#include <simplechess-c/simplechess.h>
#include <gtest/gtest.h>

#include <cstring>

// Helper functions for C interface tests

// Helper function to create a move
static inline simple_chess_piece_move_t create_move(simple_chess_piece_type_t piece_type, simple_chess_color_t color,
                                      uint8_t src_rank, char src_file,
                                      uint8_t dst_rank, char dst_file) {
    simple_chess_piece_move_t move = {};  // Zero-initialize
    move.piece.type = piece_type;
    move.piece.color = color;

    // Initialize source square
    move.src.rank = src_rank;
    move.src.file = src_file;

    // Initialize destination square
    move.dst.rank = dst_rank;
    move.dst.file = dst_file;

    move.is_promotion = false;
    move.promoted_to = SIMPLE_CHESS_PIECE_TYPE_PAWN; // dummy value
    return move;
}

// Helper function to create a promotion move
static inline simple_chess_piece_move_t create_promotion_move(simple_chess_color_t color,
                                                uint8_t src_rank, char src_file,
                                                uint8_t dst_rank, char dst_file,
                                                simple_chess_piece_type_t promoted_to) {
    simple_chess_piece_move_t move = {};  // Zero-initialize
    move.piece.type = SIMPLE_CHESS_PIECE_TYPE_PAWN;
    move.piece.color = color;

    // Initialize source square
    move.src.rank = src_rank;
    move.src.file = src_file;

    // Initialize destination square
    move.dst.rank = dst_rank;
    move.dst.file = dst_file;

    move.is_promotion = true;
    move.promoted_to = promoted_to;
    return move;
}

// Helper to check if a game is not null (since C interface returns null on errors)
#define ASSERT_GAME_NOT_NULL(game) \
    ASSERT_NE(game, nullptr) << "Game creation failed - C interface returned null"

#define EXPECT_GAME_NOT_NULL(game) \
    EXPECT_NE(game, nullptr) << "Game operation failed - C interface returned null"

// A game is opaque, so its contents come out through accessors which report
// success separately from the value they fetch. The tests read one thing at a
// time and would rather not declare a temporary for each, so these return the
// value directly and are only used where it is known to exist.
static inline simple_chess_game_stage_t current_stage(const simple_chess_game_t* game) {
    simple_chess_game_stage_t stage = {};
    EXPECT_TRUE(simple_chess_game_current_stage(game, &stage));
    return stage;
}

static inline simple_chess_piece_move_t available_move(
        const simple_chess_game_t* game, uint16_t index) {
    simple_chess_piece_move_t move = {};
    EXPECT_TRUE(simple_chess_game_available_move(game, index, &move));
    return move;
}

static inline simple_chess_played_move_t history_move(
        const simple_chess_game_t* game, uint16_t index) {
    simple_chess_played_move_t move = {};
    EXPECT_TRUE(simple_chess_game_history_move(game, index, &move));
    return move;
}

static inline bool is_draw_claimable(const simple_chess_game_t* game) {
    simple_chess_draw_reason_t reason;
    return simple_chess_game_reason_to_claim_draw(game, &reason);
}

static inline simple_chess_draw_reason_t reason_to_claim_draw(
        const simple_chess_game_t* game) {
    simple_chess_draw_reason_t reason = {};
    EXPECT_TRUE(simple_chess_game_reason_to_claim_draw(game, &reason));
    return reason;
}

static inline simple_chess_draw_reason_t draw_reason(const simple_chess_game_t* game) {
    simple_chess_draw_reason_t reason = {};
    EXPECT_TRUE(simple_chess_game_draw_reason(game, &reason));
    return reason;
}

// Helper to check the last played move's algebraic notation
#define EXPECT_LAST_MOVE_NOTATION(game, expected) \
    do { \
        ASSERT_GT(simple_chess_game_history_size(game), 0) << "No moves in history"; \
        EXPECT_STREQ( \
            history_move(game, simple_chess_game_history_size(game) - 1) \
                .in_algebraic_notation, \
            expected); \
    } while(0)

#endif // TEST_UTILS_C_H
