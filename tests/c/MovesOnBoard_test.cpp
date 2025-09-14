#include "TestUtils.h"

// Helper function to check if a piece is at a specific position
static bool is_piece_at_square(const board_t* board, uint8_t rank, char file, piece_type_t type, color_t color) {
    uint8_t index = (rank - 1) * 8 + (file - 'a');
    if (!board->occupied[index]) {
        return false;
    }
    return board->piece_at[index].type == type && board->piece_at[index].color == color;
}

// Helper function to check if a square is empty
static bool is_square_empty(const board_t* board, uint8_t rank, char file) {
    uint8_t index = (rank - 1) * 8 + (file - 'a');
    return !board->occupied[index];
}

// Helper function for testing regular non-capture moves
static void test_regular_non_capture_move(const char* fen,
                                        piece_type_t piece_type, color_t color,
                                        uint8_t src_rank, char src_file,
                                        uint8_t dst_rank, char dst_file) {
    game_t* game = simple_chess_create_game_from_fen(fen);
    ASSERT_GAME_NOT_NULL(game);

    // Verify the piece is at source position before move
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, src_rank, src_file, piece_type, color));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, dst_rank, dst_file));

    piece_move_t move = create_move(piece_type, color, src_rank, src_file, dst_rank, dst_file);
    game_t* after_move = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify the piece moved to destination and source is empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, src_rank, src_file));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, dst_rank, dst_file, piece_type, color));

    destroy_game(game);
    destroy_game(after_move);
}

// Helper function for testing regular capture moves
static void test_regular_capture_move(const char* fen,
                                    piece_type_t piece_type, color_t color,
                                    uint8_t src_rank, char src_file,
                                    uint8_t dst_rank, char dst_file) {
    game_t* game = simple_chess_create_game_from_fen(fen);
    ASSERT_GAME_NOT_NULL(game);

    // Verify the piece is at source position before move
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, src_rank, src_file, piece_type, color));
    EXPECT_FALSE(is_square_empty(&game->current_stage.board, dst_rank, dst_file));

    piece_move_t move = create_move(piece_type, color, src_rank, src_file, dst_rank, dst_file);
    game_t* after_move = simple_chess_make_move(game, move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify the piece moved to destination and source is empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, src_rank, src_file));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, dst_rank, dst_file, piece_type, color));

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, PawnOnceForward) {
    test_regular_non_capture_move(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        PieceTypePawn, ColorWhite, 2, 'f', 3, 'f');
}

TEST(CMovesOnBoardTest, PawnTwiceForward) {
    test_regular_non_capture_move(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        PieceTypePawn, ColorWhite, 2, 'a', 4, 'a');
}

TEST(CMovesOnBoardTest, PawnCapture) {
    test_regular_capture_move(
        "rn1qk2r/ppp2ppp/3p1n2/4p3/3P2b1/2N1P3/PPPBQPPP/R3KBNR b KQkq - 0 1",
        PieceTypePawn, ColorBlack, 5, 'e', 4, 'd');
}

TEST(CMovesOnBoardTest, WhitePawnEnPassant) {
    game_t* game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: white pawn on d5, black pawn on e5
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 5, 'd', PieceTypePawn, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 5, 'e', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 6, 'e'));

    piece_move_t en_passant_move = create_move(PieceTypePawn, ColorWhite, 5, 'd', 6, 'e');
    game_t* after_move = simple_chess_make_move(game, en_passant_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify en passant capture: white pawn on e6, black pawn captured, d5 empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 5, 'd'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 6, 'e', PieceTypePawn, ColorWhite));
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 5, 'e')); // Black pawn captured

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, BlackPawnEnPassant) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/6pP/8/1K6/8 b - h3 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: black pawn on g4, white pawn on h4
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 4, 'g', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 4, 'h', PieceTypePawn, ColorWhite));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 3, 'h'));

    piece_move_t en_passant_move = create_move(PieceTypePawn, ColorBlack, 4, 'g', 3, 'h');
    game_t* after_move = simple_chess_make_move(game, en_passant_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify en passant capture: black pawn on h3, white pawn captured, g4 empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 4, 'g'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 3, 'h', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 4, 'h')); // White pawn captured

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, PawnPromotionNoCapture) {
    game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/2q5/7P/2RQ4/1K4p1/8 b - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: black pawn on g2
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 2, 'g', PieceTypePawn, ColorBlack));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 1, 'g'));

    piece_move_t promotion_move = create_promotion_move(ColorBlack, 2, 'g', 1, 'g', PieceTypeQueen);
    game_t* after_move = simple_chess_make_move(game, promotion_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify promotion: g2 empty, queen on g1
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 2, 'g'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 1, 'g', PieceTypeQueen, ColorBlack));

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, PawnPromotionCapture) {
    game_t* game = simple_chess_create_game_from_fen(
        "2q5/1P2k3/8/8/8/2RQ4/1K4p1/8 w - - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: white pawn on b7, black queen on c8
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 7, 'b', PieceTypePawn, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 8, 'c', PieceTypeQueen, ColorBlack));

    piece_move_t promotion_move = create_promotion_move(ColorWhite, 7, 'b', 8, 'c', PieceTypeQueen);
    game_t* after_move = simple_chess_make_move(game, promotion_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify promotion with capture: b7 empty, white queen on c8
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 7, 'b'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 8, 'c', PieceTypeQueen, ColorWhite));

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, KnightNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1",
        PieceTypeKnight, ColorBlack, 5, 'c', 3, 'b');
}

TEST(CMovesOnBoardTest, KnightCapture) {
    test_regular_capture_move(
        "8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1",
        PieceTypeKnight, ColorBlack, 5, 'c', 3, 'd');
}

TEST(CMovesOnBoardTest, BishopNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1",
        PieceTypeBishop, ColorWhite, 3, 'd', 1, 'f');
}

TEST(CMovesOnBoardTest, BishopCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1",
        PieceTypeBishop, ColorWhite, 3, 'd', 6, 'g');
}

TEST(CMovesOnBoardTest, RookNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1",
        PieceTypeRook, ColorWhite, 3, 'c', 4, 'c');
}

TEST(CMovesOnBoardTest, RookCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1",
        PieceTypeRook, ColorWhite, 3, 'c', 5, 'c');
}

TEST(CMovesOnBoardTest, QueenNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1",
        PieceTypeQueen, ColorBlack, 5, 'c', 2, 'f');
}

TEST(CMovesOnBoardTest, QueenCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1",
        PieceTypeQueen, ColorBlack, 5, 'c', 3, 'c');
}

TEST(CMovesOnBoardTest, KingNoCapture) {
    test_regular_non_capture_move(
        "2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1",
        PieceTypeKing, ColorBlack, 8, 'c', 8, 'b');
}

TEST(CMovesOnBoardTest, KingCapture) {
    test_regular_capture_move(
        "2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1",
        PieceTypeKing, ColorBlack, 8, 'c', 7, 'b');
}

TEST(CMovesOnBoardTest, KingsideCastlingWhite) {
    game_t* game = simple_chess_create_game_from_fen(
        "r2qkbnr/ppp2ppp/2np4/1B2p3/6b1/4PN2/PPPP1PPP/RNBQK2R w KQkq - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: king on e1, rook on h1
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 1, 'e', PieceTypeKing, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 1, 'h', PieceTypeRook, ColorWhite));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 1, 'f'));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 1, 'g'));

    piece_move_t castle_move = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'g');
    game_t* after_move = simple_chess_make_move(game, castle_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify castling: king on g1, rook on f1, e1 and h1 empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 1, 'e'));
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 1, 'h'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 1, 'g', PieceTypeKing, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 1, 'f', PieceTypeRook, ColorWhite));

    destroy_game(game);
    destroy_game(after_move);
}

TEST(CMovesOnBoardTest, QueensideCastlingWhite) {
    game_t* game = simple_chess_create_game_from_fen(
        "r2qkbnr/ppp2ppp/2np4/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR w KQkq - 0 1");
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: king on e1, rook on a1
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 1, 'e', PieceTypeKing, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&game->current_stage.board, 1, 'a', PieceTypeRook, ColorWhite));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 1, 'c'));
    EXPECT_TRUE(is_square_empty(&game->current_stage.board, 1, 'd'));

    piece_move_t castle_move = create_move(PieceTypeKing, ColorWhite, 1, 'e', 1, 'c');
    game_t* after_move = simple_chess_make_move(game, castle_move);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify castling: king on c1, rook on d1, e1 and a1 empty
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 1, 'e'));
    EXPECT_TRUE(is_square_empty(&after_move->current_stage.board, 1, 'a'));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 1, 'c', PieceTypeKing, ColorWhite));
    EXPECT_TRUE(is_piece_at_square(&after_move->current_stage.board, 1, 'd', PieceTypeRook, ColorWhite));

    destroy_game(game);
    destroy_game(after_move);
}