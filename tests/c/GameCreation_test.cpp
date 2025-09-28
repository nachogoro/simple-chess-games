#include "TestUtils.h"

// Helper function to check piece at specific board position
bool check_piece_at(const board_t* board, uint8_t rank, char file, piece_type_t expected_type, color_t expected_color) {
    uint8_t index = simple_chess_index_from_square({rank, file});
    if (!board->occupied[index]) return false;
    return board->piece_at[index].type == expected_type &&
           board->piece_at[index].color == expected_color;
}

TEST(CGameCreationTest, RegularGameCreation) {
    game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, GameStatePlaying);
    EXPECT_EQ(game->history_size, 0);
    EXPECT_EQ(game->current_stage.active_color, ColorWhite);
    EXPECT_FALSE(game->is_draw_claimable);

    // Validate piece positions - check a few key pieces
    const board_t* board = &game->current_stage.board;

    // Black pieces on rank 8
    EXPECT_TRUE(check_piece_at(board, 8, 'a', PieceTypeRook, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'b', PieceTypeKnight, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'c', PieceTypeBishop, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'd', PieceTypeQueen, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'e', PieceTypeKing, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'f', PieceTypeBishop, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'g', PieceTypeKnight, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'h', PieceTypeRook, ColorBlack));

    // Black pawns on rank 7
    for (char file = 'a'; file <= 'h'; file++) {
        EXPECT_TRUE(check_piece_at(board, 7, file, PieceTypePawn, ColorBlack));
    }

    // White pieces on rank 1
    EXPECT_TRUE(check_piece_at(board, 1, 'a', PieceTypeRook, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'b', PieceTypeKnight, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'c', PieceTypeBishop, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'd', PieceTypeQueen, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'e', PieceTypeKing, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'f', PieceTypeBishop, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'g', PieceTypeKnight, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 1, 'h', PieceTypeRook, ColorWhite));

    // White pawns on rank 2
    for (char file = 'a'; file <= 'h'; file++) {
        EXPECT_TRUE(check_piece_at(board, 2, file, PieceTypePawn, ColorWhite));
    }

    // Check empty squares in the middle
    for (uint8_t rank = 3; rank <= 6; rank++) {
        for (char file = 'a'; file <= 'h'; file++) {
            uint8_t index = simple_chess_index_from_square({rank, file});
            EXPECT_FALSE(board->occupied[index]) << "Square " << file << rank << " should be empty";
        }
    }

    destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPosition1) {
    game_t* game = simple_chess_create_game_from_fen(
        "5rk1/3Q1p1p/6p1/8/3B4/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, GameStatePlaying);
    EXPECT_EQ(game->history_size, 0);
    EXPECT_EQ(game->current_stage.active_color, ColorBlack);
    EXPECT_FALSE(game->is_draw_claimable);

    // Validate piece positions
    const board_t* board = &game->current_stage.board;
    EXPECT_TRUE(check_piece_at(board, 8, 'f', PieceTypeRook, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 8, 'g', PieceTypeKing, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 7, 'd', PieceTypeQueen, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 7, 'f', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 7, 'h', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 6, 'g', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(check_piece_at(board, 4, 'd', PieceTypeBishop, ColorWhite));
    EXPECT_TRUE(check_piece_at(board, 3, 'e', PieceTypeKing, ColorWhite));

    destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPositionInCheckmate) {
    game_t* game = simple_chess_create_game_from_fen(
        "6kr/5Q1p/3N2p1/8/8/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, GameStateWhiteWon);

    destroy_game(game);
}

TEST(CGameCreationTest, GameCreationFromPositionInStalemate) {
    game_t* game = simple_chess_create_game_from_fen(
        "7k/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(game->state, GameStateDrawn);
    EXPECT_EQ(game->draw_reason, DrawReasonStaleMate);

    destroy_game(game);
}

TEST(CGameCreationTest, GameCreationWithTooManyKings) {
    game_t* game = simple_chess_create_game_from_fen(
        "5kk1/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationWithTooFewKings) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationActiveSideAlreadyChecking) {
    game_t* game = simple_chess_create_game_from_fen(
        "k4n2/5n1K/8/8/8/8/8/6r1 b - - 0 1");

    // C interface should return null on invalid FEN
    EXPECT_EQ(game, nullptr);
}

TEST(CGameCreationTest, GameCreationWithNullFen) {
    game_t* game = simple_chess_create_game_from_fen(nullptr);

    // C interface should return null when passed null FEN
    EXPECT_EQ(game, nullptr);
}