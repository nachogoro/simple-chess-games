#include "TestUtils.h"

// Helper function to check if a move is in the available moves array
static bool is_move_available(const game_t* game, const piece_move_t& move) {
    for (uint16_t i = 0; i < game->available_move_count; i++) {
        const piece_move_t& available = game->available_moves[i];
        if (available.piece.type == move.piece.type &&
            available.piece.color == move.piece.color &&
            available.src.rank == move.src.rank &&
            available.src.file == move.src.file &&
            available.dst.rank == move.dst.rank &&
            available.dst.file == move.dst.file &&
            available.is_promotion == move.is_promotion) {
            if (!move.is_promotion || available.promoted_to == move.promoted_to) {
                return true;
            }
        }
    }
    return false;
}

TEST(CMoveAvailabilityTest, RegularGameMoves) {
    game_t* game = simple_chess_create_new_game();
    ASSERT_GAME_NOT_NULL(game);

    // New game should have 20 possible moves (16 pawn moves + 4 knight moves)
    EXPECT_EQ(game->available_move_count, 20);

    // Check that specific starting moves are available
    piece_move_t pawn_e2_e4 = create_move(PieceTypePawn, ColorWhite, 2, 'e', 4, 'e');
    EXPECT_TRUE(is_move_available(game, pawn_e2_e4));

    piece_move_t pawn_e2_e3 = create_move(PieceTypePawn, ColorWhite, 2, 'e', 3, 'e');
    EXPECT_TRUE(is_move_available(game, pawn_e2_e3));

    piece_move_t knight_g1_f3 = create_move(PieceTypeKnight, ColorWhite, 1, 'g', 3, 'f');
    EXPECT_TRUE(is_move_available(game, knight_g1_f3));

    piece_move_t knight_b1_c3 = create_move(PieceTypeKnight, ColorWhite, 1, 'b', 3, 'c');
    EXPECT_TRUE(is_move_available(game, knight_b1_c3));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, KnightMovesUnobstructed) {
    game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/8/3N4/8/8/K7 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Knight on d4 should have 8 possible moves + king moves (3)
    EXPECT_EQ(game->available_move_count, 11);

    // Check some specific knight moves
    piece_move_t knight_d4_c6 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 6, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c6));

    piece_move_t knight_d4_e6 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 6, 'e');
    EXPECT_TRUE(is_move_available(game, knight_d4_e6));

    piece_move_t knight_d4_f5 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 5, 'f');
    EXPECT_TRUE(is_move_available(game, knight_d4_f5));

    piece_move_t knight_d4_c2 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 2, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c2));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, KnightMovesObstructedAttempt) {
    // This test proves that the knight is unaffected by pieces "on its path"
    // to its destination squares
    game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/2rrr3/2rNr3/2rrr3/8/K7 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Knight on d4 should still have 8 possible moves + king moves (3)
    EXPECT_EQ(game->available_move_count, 11);

    // Check some specific knight moves are still available despite obstruction
    piece_move_t knight_d4_c6 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 6, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c6));

    piece_move_t knight_d4_e6 = create_move(PieceTypeKnight, ColorWhite, 4, 'd', 6, 'e');
    EXPECT_TRUE(is_move_available(game, knight_d4_e6));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, RookMovesUnobstructed) {
    game_t* game = simple_chess_create_game_from_fen(
        "4k3/8/8/3R4/8/8/8/4K3 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Rook on d5 should have 14 moves + king moves (5) = 19 total
    EXPECT_EQ(game->available_move_count, 19);

    // Check some specific rook moves
    piece_move_t rook_d5_d8 = create_move(PieceTypeRook, ColorWhite, 5, 'd', 8, 'd');
    EXPECT_TRUE(is_move_available(game, rook_d5_d8));

    piece_move_t rook_d5_d1 = create_move(PieceTypeRook, ColorWhite, 5, 'd', 1, 'd');
    EXPECT_TRUE(is_move_available(game, rook_d5_d1));

    piece_move_t rook_d5_a5 = create_move(PieceTypeRook, ColorWhite, 5, 'd', 5, 'a');
    EXPECT_TRUE(is_move_available(game, rook_d5_a5));

    piece_move_t rook_d5_h5 = create_move(PieceTypeRook, ColorWhite, 5, 'd', 5, 'h');
    EXPECT_TRUE(is_move_available(game, rook_d5_h5));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, RookMovesObstructed) {
    game_t* game = simple_chess_create_game_from_fen(
        "7k/r5r1/3r4/8/1r1R2r1/8/1r3r2/2K5 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Rook on d4 with obstructions should have limited moves + king move (1) = 11 total
    EXPECT_EQ(game->available_move_count, 11);

    // Check some specific available rook moves
    piece_move_t rook_d4_c4 = create_move(PieceTypeRook, ColorWhite, 4, 'd', 4, 'c');
    EXPECT_TRUE(is_move_available(game, rook_d4_c4));

    piece_move_t rook_d4_e4 = create_move(PieceTypeRook, ColorWhite, 4, 'd', 4, 'e');
    EXPECT_TRUE(is_move_available(game, rook_d4_e4));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingUnobstructed) {
    game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/8/8/8/8/R3K2R w KQ - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Should include both castling moves
    EXPECT_GT(game->available_move_count, 20);

    // Check that king-side castling is available
    piece_move_t king_side_castle = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'g');
    EXPECT_TRUE(is_move_available(game, king_side_castle));

    // Check that queen-side castling is available
    piece_move_t queen_side_castle = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'c');
    EXPECT_TRUE(is_move_available(game, queen_side_castle));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingUnavailable) {
    game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/8/8/8/8/R3K2R w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Castling should not be available (no castling rights in FEN)
    piece_move_t king_side_castle = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'g');
    EXPECT_FALSE(is_move_available(game, king_side_castle));

    piece_move_t queen_side_castle = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'c');
    EXPECT_FALSE(is_move_available(game, queen_side_castle));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, EnPassantAvailable) {
    game_t* game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppp1ppp/8/8/4pP2/4P3/PPPP2PP/RNBQKBNR b KQkq f3 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // En passant move should be available
    piece_move_t en_passant = create_move(PieceTypePawn, ColorBlack, 4, 'e', 3, 'f');
    EXPECT_TRUE(is_move_available(game, en_passant));

    destroy_game(game);
}

TEST(CMoveAvailabilityTest, PawnPromotion) {
    game_t* game = simple_chess_create_game_from_fen(
        "2k1n3/5Pb1/8/3p4/8/K7/8/8 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Should have promotion moves (4 for straight, 4 for capture) + other moves
    EXPECT_GT(game->available_move_count, 8);

    // Check promotion to queen (straight move)
    piece_move_t promotion_queen = create_promotion_move(ColorWhite, 7, 'f', 8, 'f', PieceTypeQueen);
    EXPECT_TRUE(is_move_available(game, promotion_queen));

    // Check promotion to knight (straight move)
    piece_move_t promotion_knight = create_promotion_move(ColorWhite, 7, 'f', 8, 'f', PieceTypeKnight);
    EXPECT_TRUE(is_move_available(game, promotion_knight));

    // Check promotion with capture
    piece_move_t promotion_capture = create_promotion_move(ColorWhite, 7, 'f', 8, 'e', PieceTypeQueen);
    EXPECT_TRUE(is_move_available(game, promotion_capture));

    destroy_game(game);
}