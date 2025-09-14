#ifndef TEST_UTILS_C_H
#define TEST_UTILS_C_H

#include <c/simplechess/simplechess.h>
#include <gtest/gtest.h>

// Helper functions for C interface tests

// Helper function to create a move
static inline piece_move_t create_move(piece_type_t piece_type, color_t color,
                                      uint8_t src_rank, char src_file,
                                      uint8_t dst_rank, char dst_file) {
    piece_move_t move = {};  // Zero-initialize
    move.piece.type = piece_type;
    move.piece.color = color;

    // Initialize source square
    move.src.rank = src_rank;
    move.src.file = src_file;

    // Initialize destination square
    move.dst.rank = dst_rank;
    move.dst.file = dst_file;

    move.is_promotion = false;
    move.promoted_to = PieceTypePawn; // dummy value
    return move;
}

// Helper function to create a promotion move
static inline piece_move_t create_promotion_move(color_t color,
                                                uint8_t src_rank, char src_file,
                                                uint8_t dst_rank, char dst_file,
                                                piece_type_t promoted_to) {
    piece_move_t move = {};  // Zero-initialize
    move.piece.type = PieceTypePawn;
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

// Helper to check the last played move's algebraic notation
#define EXPECT_LAST_MOVE_NOTATION(game, expected) \
    do { \
        ASSERT_GT(game->history_size, 0) << "No moves in history"; \
        EXPECT_STREQ(game->history[game->history_size - 1].played_move.in_algebraic_notation, expected); \
    } while(0)

#endif // TEST_UTILS_C_H
