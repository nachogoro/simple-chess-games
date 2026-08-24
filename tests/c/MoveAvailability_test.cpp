#include "TestUtils.h"

// Helper function to check if a move is in the available moves array
static bool is_move_available(const simple_chess_game_t* game, const simple_chess_piece_move_t& move) {
    for (uint16_t i = 0; i < simple_chess_game_available_move_count(game); i++) {
        if (simple_chess_piece_move_equals(available_move(game, i), move)) {
            return true;
        }
    }
    return false;
}

TEST(CMoveAvailabilityTest, RegularGameMoves) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // New game should have 20 possible moves (16 pawn moves + 4 knight moves)
    EXPECT_EQ(simple_chess_game_available_move_count(game), 20);

    // Check that specific starting moves are available
    simple_chess_piece_move_t pawn_e2_e4 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'e', 4, 'e');
    EXPECT_TRUE(is_move_available(game, pawn_e2_e4));

    simple_chess_piece_move_t pawn_e2_e3 = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'e', 3, 'e');
    EXPECT_TRUE(is_move_available(game, pawn_e2_e3));

    simple_chess_piece_move_t knight_g1_f3 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f');
    EXPECT_TRUE(is_move_available(game, knight_g1_f3));

    simple_chess_piece_move_t knight_b1_c3 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'b', 3, 'c');
    EXPECT_TRUE(is_move_available(game, knight_b1_c3));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, KnightMovesUnobstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/8/3N4/8/8/K7 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Knight on d4 should have 8 possible moves + king moves (3)
    EXPECT_EQ(simple_chess_game_available_move_count(game), 11);

    // Check some specific knight moves
    simple_chess_piece_move_t knight_d4_c6 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 6, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c6));

    simple_chess_piece_move_t knight_d4_e6 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 6, 'e');
    EXPECT_TRUE(is_move_available(game, knight_d4_e6));

    simple_chess_piece_move_t knight_d4_f5 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 5, 'f');
    EXPECT_TRUE(is_move_available(game, knight_d4_f5));

    simple_chess_piece_move_t knight_d4_c2 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 2, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c2));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, KnightMovesObstructedAttempt) {
    // This test proves that the knight is unaffected by pieces "on its path"
    // to its destination squares
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/8/8/2rrr3/2rNr3/2rrr3/8/K7 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Knight on d4 should still have 8 possible moves + king moves (3)
    EXPECT_EQ(simple_chess_game_available_move_count(game), 11);

    // Check some specific knight moves are still available despite obstruction
    simple_chess_piece_move_t knight_d4_c6 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 6, 'c');
    EXPECT_TRUE(is_move_available(game, knight_d4_c6));

    simple_chess_piece_move_t knight_d4_e6 = create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 6, 'e');
    EXPECT_TRUE(is_move_available(game, knight_d4_e6));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, BishopMovesUnobstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "3k4/8/8/3BB3/8/8/8/3K4 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Two bishops with 13 moves each + king moves (5)
    EXPECT_EQ(simple_chess_game_available_move_count(game), 31);

    // Each bishop reaches the end of all four of its diagonals
    simple_chess_piece_move_t bishop_d5_a8 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 8, 'a');
    EXPECT_TRUE(is_move_available(game, bishop_d5_a8));

    simple_chess_piece_move_t bishop_d5_h1 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 1, 'h');
    EXPECT_TRUE(is_move_available(game, bishop_d5_h1));

    simple_chess_piece_move_t bishop_e5_a1 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 5, 'e', 1, 'a');
    EXPECT_TRUE(is_move_available(game, bishop_e5_a1));

    simple_chess_piece_move_t bishop_e5_h8 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 5, 'e', 8, 'h');
    EXPECT_TRUE(is_move_available(game, bishop_e5_h8));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, BishopMovesObstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/r5r1/3r4/8/1r1B2r1/8/1r3r2/2K5 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Every diagonal ends on a rook (10 moves), and the king has only d1
    EXPECT_EQ(simple_chess_game_available_move_count(game), 11);

    // The rook closing each diagonal can be captured
    simple_chess_piece_move_t bishop_d4_a7 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 7, 'a');
    EXPECT_TRUE(is_move_available(game, bishop_d4_a7));

    simple_chess_piece_move_t bishop_d4_b2 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 2, 'b');
    EXPECT_TRUE(is_move_available(game, bishop_d4_b2));

    // But nothing behind it is reachable
    simple_chess_piece_move_t bishop_d4_a1 = create_move(SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 1, 'a');
    EXPECT_FALSE(is_move_available(game, bishop_d4_a1));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, RookMovesUnobstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "4k3/8/8/3R4/8/8/8/4K3 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Rook on d5 should have 14 moves + king moves (5) = 19 total
    EXPECT_EQ(simple_chess_game_available_move_count(game), 19);

    // Check some specific rook moves
    simple_chess_piece_move_t rook_d5_d8 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 8, 'd');
    EXPECT_TRUE(is_move_available(game, rook_d5_d8));

    simple_chess_piece_move_t rook_d5_d1 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 1, 'd');
    EXPECT_TRUE(is_move_available(game, rook_d5_d1));

    simple_chess_piece_move_t rook_d5_a5 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 5, 'a');
    EXPECT_TRUE(is_move_available(game, rook_d5_a5));

    simple_chess_piece_move_t rook_d5_h5 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 5, 'h');
    EXPECT_TRUE(is_move_available(game, rook_d5_h5));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, RookMovesObstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/r5r1/3r4/8/1r1R2r1/8/1r3r2/2K5 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Rook on d4 with obstructions should have limited moves + king move (1) = 11 total
    EXPECT_EQ(simple_chess_game_available_move_count(game), 11);

    // Check some specific available rook moves
    simple_chess_piece_move_t rook_d4_c4 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 4, 'c');
    EXPECT_TRUE(is_move_available(game, rook_d4_c4));

    simple_chess_piece_move_t rook_d4_e4 = create_move(SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 4, 'e');
    EXPECT_TRUE(is_move_available(game, rook_d4_e4));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, QueenMovesUnobstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "4k3/8/8/3Q4/8/8/8/4K3 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Queen on d5 has 27 moves (13 diagonal + 14 straight) + king moves (5)
    EXPECT_EQ(simple_chess_game_available_move_count(game), 32);

    // The queen moves as both bishop and rook
    simple_chess_piece_move_t queen_d5_a8 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 8, 'a');
    EXPECT_TRUE(is_move_available(game, queen_d5_a8));

    simple_chess_piece_move_t queen_d5_h1 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 1, 'h');
    EXPECT_TRUE(is_move_available(game, queen_d5_h1));

    simple_chess_piece_move_t queen_d5_d8 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 8, 'd');
    EXPECT_TRUE(is_move_available(game, queen_d5_d8));

    simple_chess_piece_move_t queen_d5_h5 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 5, 'h');
    EXPECT_TRUE(is_move_available(game, queen_d5_h5));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, QueenMovesObstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "7k/r5r1/3r4/8/1r1Q2r1/8/1r3r2/2K5 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // The same position as the obstructed bishop, with a queen on d4: 20
    // moves, plus the king's single one
    EXPECT_EQ(simple_chess_game_available_move_count(game), 21);

    // The rook closing each line can be captured, diagonally and straight
    simple_chess_piece_move_t queen_d4_a7 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 7, 'a');
    EXPECT_TRUE(is_move_available(game, queen_d4_a7));

    simple_chess_piece_move_t queen_d4_d6 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 6, 'd');
    EXPECT_TRUE(is_move_available(game, queen_d4_d6));

    simple_chess_piece_move_t queen_d4_b4 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 4, 'b');
    EXPECT_TRUE(is_move_available(game, queen_d4_b4));

    simple_chess_piece_move_t queen_d4_g4 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 4, 'g');
    EXPECT_TRUE(is_move_available(game, queen_d4_g4));

    // But not the square behind one of them
    simple_chess_piece_move_t queen_d4_d7 = create_move(SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE, 4, 'd', 7, 'd');
    EXPECT_FALSE(is_move_available(game, queen_d4_d7));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingUnobstructed) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/8/8/8/8/R3K2R w KQ - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Should include both castling moves
    EXPECT_GT(simple_chess_game_available_move_count(game), 20);

    // Check that king-side castling is available
    simple_chess_piece_move_t king_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g');
    EXPECT_TRUE(is_move_available(game, king_side_castle));

    // Check that queen-side castling is available
    simple_chess_piece_move_t queen_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'c');
    EXPECT_TRUE(is_move_available(game, queen_side_castle));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingQueensideObstructed) {
    // The bishop on g5 guards c1, which the king would land on
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/6b1/8/8/8/R3K2R w KQ - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_available_move_count(game), 24);

    simple_chess_piece_move_t king_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g');
    EXPECT_TRUE(is_move_available(game, king_side_castle));

    simple_chess_piece_move_t queen_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'c');
    EXPECT_FALSE(is_move_available(game, queen_side_castle));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingBothObstructed) {
    // A queen on the same square guards g1 down the file as well as c1
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/6q1/8/8/8/R3K2R w KQ - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    EXPECT_EQ(simple_chess_game_available_move_count(game), 23);

    simple_chess_piece_move_t king_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g');
    EXPECT_FALSE(is_move_available(game, king_side_castle));

    simple_chess_piece_move_t queen_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'c');
    EXPECT_FALSE(is_move_available(game, queen_side_castle));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, WhiteCastlingUnavailable) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "1k6/8/8/8/8/8/8/R3K2R w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Castling should not be available (no castling rights in FEN)
    simple_chess_piece_move_t king_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'g');
    EXPECT_FALSE(is_move_available(game, king_side_castle));

    simple_chess_piece_move_t queen_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_WHITE, 1, 'e', 1, 'c');
    EXPECT_FALSE(is_move_available(game, queen_side_castle));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, BlackCastlingInCheck) {
    // A king in check may not castle out of it, on either side
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "4k2r/8/8/8/8/2K5/8/4R3 b k - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t king_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'e', 8, 'g');
    EXPECT_FALSE(is_move_available(game, king_side_castle));

    simple_chess_piece_move_t queen_side_castle = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'e', 8, 'c');
    EXPECT_FALSE(is_move_available(game, queen_side_castle));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, EnPassantAvailable) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppp1ppp/8/8/4pP2/4P3/PPPP2PP/RNBQKBNR b KQkq f3 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // En passant move should be available
    simple_chess_piece_move_t en_passant = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 4, 'e', 3, 'f');
    EXPECT_TRUE(is_move_available(game, en_passant));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, EnPassantWouldLeaveInCheck) {
    // Capturing en passant vacates two squares at once, which here opens the
    // bishop's diagonal onto the white king
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "2k5/6b1/8/3pP3/8/8/1K6/8 w - d6 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t en_passant = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 5, 'e', 6, 'd');
    EXPECT_FALSE(is_move_available(game, en_passant));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, PawnPromotion) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "2k1n3/5Pb1/8/3p4/8/K7/8/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Should have promotion moves (4 for straight, 4 for capture) + other moves
    EXPECT_GT(simple_chess_game_available_move_count(game), 8);

    // Check promotion to queen (straight move)
    simple_chess_piece_move_t promotion_queen = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'f', 8, 'f', SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    EXPECT_TRUE(is_move_available(game, promotion_queen));

    // Check promotion to knight (straight move)
    simple_chess_piece_move_t promotion_knight = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'f', 8, 'f', SIMPLE_CHESS_PIECE_TYPE_KNIGHT);
    EXPECT_TRUE(is_move_available(game, promotion_knight));

    // Check promotion with capture
    simple_chess_piece_move_t promotion_capture = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'f', 8, 'e', SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    EXPECT_TRUE(is_move_available(game, promotion_capture));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, FindMoveBySourceAndDestination) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // The caller does not have to know which piece stands on g1.
    simple_chess_piece_move_t knight = {};
    ASSERT_TRUE(simple_chess_find_move(
                game, {1, 'g'}, {3, 'f'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &knight));

    EXPECT_EQ(knight.piece.type, SIMPLE_CHESS_PIECE_TYPE_KNIGHT);
    EXPECT_EQ(knight.piece.color, SIMPLE_CHESS_COLOR_WHITE);
    EXPECT_EQ(knight.src.rank, 1);
    EXPECT_EQ(knight.src.file, 'g');
    EXPECT_EQ(knight.dst.rank, 3);
    EXPECT_EQ(knight.dst.file, 'f');
    EXPECT_FALSE(knight.is_promotion);

    // A move which is not legal in this position is not found, even though
    // both squares exist.
    simple_chess_piece_move_t out = {};
    EXPECT_FALSE(simple_chess_find_move(
                game, {2, 'e'}, {5, 'e'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &out));

    // Nor is a move of a piece belonging to the player who is not to play.
    EXPECT_FALSE(simple_chess_find_move(
                game, {7, 'e'}, {5, 'e'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &out));

    simple_chess_destroy_game(game);
}

TEST(CMoveAvailabilityTest, FindMoveDistinguishesPromotions) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "k7/5P2/8/8/8/8/8/4K3 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t to_queen = {};
    ASSERT_TRUE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                true, SIMPLE_CHESS_PIECE_TYPE_QUEEN, &to_queen));

    simple_chess_piece_move_t to_knight = {};
    ASSERT_TRUE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                true, SIMPLE_CHESS_PIECE_TYPE_KNIGHT, &to_knight));

    EXPECT_TRUE(to_queen.is_promotion);
    EXPECT_EQ(to_queen.promoted_to, SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    EXPECT_TRUE(to_knight.is_promotion);
    EXPECT_EQ(to_knight.promoted_to, SIMPLE_CHESS_PIECE_TYPE_KNIGHT);

    // The same two squares without naming a promotion describe a move which
    // does not exist: a pawn reaching the last rank always promotes.
    simple_chess_piece_move_t out = {};
    EXPECT_FALSE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &out));

    simple_chess_destroy_game(game);
}
