#include "TestUtils.h"

// Helper function to check piece at specific board position
bool check_piece_at(const simple_chess_board_t* board, uint8_t rank, char file, simple_chess_piece_type_t expected_type, simple_chess_color_t expected_color) {
    uint8_t index = simple_chess_index_from_square({rank, file});
    simple_chess_piece_t piece;
    if (!simple_chess_square_content_piece(board->squares[index], &piece))
        return false;
    return piece.type == expected_type && piece.color == expected_color;
}

TEST(CGameCreationTest, RegularGameCreation) {
    simple_chess_game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_EQ(game->history_size, 0);
    EXPECT_EQ(game->current_stage.active_color, SIMPLE_CHESS_COLOR_WHITE);
    EXPECT_FALSE(game->is_draw_claimable);

    // Validate piece positions - check a few key pieces
    const simple_chess_board_t* board = &game->current_stage.board;

    // Black pieces on rank 8
    EXPECT_TRUE(check_piece_at(board, 8, 'a', SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'b', SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'c', SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'd', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'e', SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'f', SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'g', SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'h', SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK));

    // Black pawns on rank 7
    for (char file = 'a'; file <= 'h'; file++) {
        EXPECT_TRUE(check_piece_at(board, 7, file, SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    }

    // White pieces on rank 1
    EXPECT_TRUE(check_piece_at(board, 1, 'a', SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'b', SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'c', SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'd', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'e', SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'f', SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'g', SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 1, 'h', SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE));

    // White pawns on rank 2
    for (char file = 'a'; file <= 'h'; file++) {
        EXPECT_TRUE(check_piece_at(board, 2, file, SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE));
    }

    // Check empty squares in the middle
    for (uint8_t rank = 3; rank <= 6; rank++) {
        for (char file = 'a'; file <= 'h'; file++) {
            uint8_t index = simple_chess_index_from_square({rank, file});
            EXPECT_EQ(board->squares[index], SIMPLE_CHESS_SQUARE_EMPTY)
                << "Square " << file << rank << " should be empty";
        }
    }

    simple_chess_destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPosition1) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "5rk1/3Q1p1p/6p1/8/3B4/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, SIMPLE_CHESS_GAME_STATE_PLAYING);
    EXPECT_EQ(game->history_size, 0);
    EXPECT_EQ(game->current_stage.active_color, SIMPLE_CHESS_COLOR_BLACK);
    EXPECT_FALSE(game->is_draw_claimable);

    // Validate piece positions
    const simple_chess_board_t* board = &game->current_stage.board;
    EXPECT_TRUE(check_piece_at(board, 8, 'f', SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 8, 'g', SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 7, 'd', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 7, 'f', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 7, 'h', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 6, 'g', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(check_piece_at(board, 4, 'd', SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(check_piece_at(board, 3, 'e', SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE));

    simple_chess_destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPositionInCheckmate) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "6kr/5Q1p/3N2p1/8/8/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, SIMPLE_CHESS_GAME_STATE_WHITE_WON);

    simple_chess_destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPositionInStalemate) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, SIMPLE_CHESS_GAME_STATE_DRAWN);
    EXPECT_EQ(game->draw_reason, SIMPLE_CHESS_DRAW_REASON_STALEMATE);

    simple_chess_destroy_game(game);
}

TEST(CGameCreationTest, GameCreationWithTooManyKings) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "5kk1/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationWithTooFewKings) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationActiveSideAlreadyChecking) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "k4n2/5n1K/8/8/8/8/8/6r1 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationWithNullFen) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(nullptr);

    // C interface should return null when passed null FEN
    EXPECT_EQ(game, nullptr);
}