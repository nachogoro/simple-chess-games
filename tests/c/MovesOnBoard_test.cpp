#include "TestUtils.h"

// Helper function to check if a piece is at a specific position
static bool is_piece_at_square(const simple_chess_board_t& board, uint8_t rank, char file, simple_chess_piece_type_t type, simple_chess_color_t color) {
    uint8_t index = (rank - 1) * 8 + (file - 'a');
    simple_chess_piece_t piece;
    if (!simple_chess_square_content_piece(board.squares[index], &piece)) {
        return false;
    }
    return piece.type == type && piece.color == color;
}

// Helper function to check if a square is empty
static bool is_square_empty(const simple_chess_board_t& board, uint8_t rank, char file) {
    uint8_t index = (rank - 1) * 8 + (file - 'a');
    return board.squares[index] == SIMPLE_CHESS_SQUARE_EMPTY;
}

// Helper function for testing regular non-capture moves
static void test_regular_non_capture_move(const char* fen,
                                        simple_chess_piece_type_t piece_type, simple_chess_color_t color,
                                        uint8_t src_rank, char src_file,
                                        uint8_t dst_rank, char dst_file) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify the piece is at source position before move
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, src_rank, src_file, piece_type, color));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, dst_rank, dst_file));

    simple_chess_piece_move_t move = create_move(piece_type, color, src_rank, src_file, dst_rank, dst_file);
    simple_chess_game_t* after_move = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify the piece moved to destination and source is empty
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, src_rank, src_file));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, dst_rank, dst_file, piece_type, color));

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

// Helper function for testing regular capture moves
static void test_regular_capture_move(const char* fen,
                                    simple_chess_piece_type_t piece_type, simple_chess_color_t color,
                                    uint8_t src_rank, char src_file,
                                    uint8_t dst_rank, char dst_file) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify the piece is at source position before move
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, src_rank, src_file, piece_type, color));
    EXPECT_FALSE(is_square_empty(current_stage(game).board, dst_rank, dst_file));

    simple_chess_piece_move_t move = create_move(piece_type, color, src_rank, src_file, dst_rank, dst_file);
    simple_chess_game_t* after_move = simple_chess_make_move(game, move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify the piece moved to destination and source is empty
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, src_rank, src_file));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, dst_rank, dst_file, piece_type, color));

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

// Helper function for testing castling, which moves two pieces at once
static void test_castling(const char* fen, simple_chess_color_t color,
                          char king_src_file, char king_dst_file,
                          char rook_src_file, char rook_dst_file) {
    const uint8_t rank = (color == SIMPLE_CHESS_COLOR_WHITE) ? 1 : 8;

    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: king and rook on their original squares, the
    // squares they move to vacant
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, rank, king_src_file, SIMPLE_CHESS_PIECE_TYPE_KING, color));
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, rank, rook_src_file, SIMPLE_CHESS_PIECE_TYPE_ROOK, color));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, rank, king_dst_file));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, rank, rook_dst_file));

    // Castling is given as the king's move alone
    simple_chess_piece_move_t castle_move = create_move(SIMPLE_CHESS_PIECE_TYPE_KING, color, rank, king_src_file, rank, king_dst_file);
    simple_chess_game_t* after_move = simple_chess_make_move(game, castle_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify castling: both pieces moved, both original squares empty
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, rank, king_src_file));
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, rank, rook_src_file));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, rank, king_dst_file, SIMPLE_CHESS_PIECE_TYPE_KING, color));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, rank, rook_dst_file, SIMPLE_CHESS_PIECE_TYPE_ROOK, color));

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

// Helper reporting whether the side to move may castle to the side its king
// lands on, which is a question about the move list rather than the board
static bool can_castle(const char* fen, char king_dst_file) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(fen,
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    EXPECT_GAME_NOT_NULL(game);

    const uint8_t rank =
        (simple_chess_game_active_color(game) == SIMPLE_CHESS_COLOR_WHITE) ? 1 : 8;

    simple_chess_piece_move_t move = {};
    const bool available = simple_chess_find_move(
            game,
            {rank, 'e'},
            {rank, king_dst_file},
            false,
            SIMPLE_CHESS_PIECE_TYPE_PAWN,
            &move);

    simple_chess_destroy_game(game);
    return available;
}

TEST(CMovesOnBoardTest, PawnOnceForward) {
    test_regular_non_capture_move(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'f', 3, 'f');
}

TEST(CMovesOnBoardTest, PawnTwiceForward) {
    test_regular_non_capture_move(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'a', 4, 'a');
}

TEST(CMovesOnBoardTest, PawnCapture) {
    test_regular_capture_move(
        "rn1qk2r/ppp2ppp/3p1n2/4p3/3P2b1/2N1P3/PPPBQPPP/R3KBNR b KQkq - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 5, 'e', 4, 'd');
}

TEST(CMovesOnBoardTest, WhitePawnEnPassant) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: white pawn on d5, black pawn on e5
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 5, 'd', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 5, 'e', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, 6, 'e'));

    simple_chess_piece_move_t en_passant_move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 5, 'd', 6, 'e');
    simple_chess_game_t* after_move = simple_chess_make_move(game, en_passant_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify en passant capture: white pawn on e6, black pawn captured, d5 empty
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 5, 'd'));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, 6, 'e', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 5, 'e')); // Black pawn captured

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CMovesOnBoardTest, BlackPawnEnPassant) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/8/6pP/8/1K6/8 b - h3 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: black pawn on g4, white pawn on h4
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 4, 'g', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 4, 'h', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, 3, 'h'));

    simple_chess_piece_move_t en_passant_move = create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 4, 'g', 3, 'h');
    simple_chess_game_t* after_move = simple_chess_make_move(game, en_passant_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify en passant capture: black pawn on h3, white pawn captured, g4 empty
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 4, 'g'));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, 3, 'h', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 4, 'h')); // White pawn captured

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CMovesOnBoardTest, PawnPromotionNoCapture) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "8/4k3/8/2q5/7P/2RQ4/1K4p1/8 b - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: black pawn on g2
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 2, 'g', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK));
    EXPECT_TRUE(is_square_empty(current_stage(game).board, 1, 'g'));

    simple_chess_piece_move_t promotion_move = create_promotion_move(SIMPLE_CHESS_COLOR_BLACK, 2, 'g', 1, 'g', SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    simple_chess_game_t* after_move = simple_chess_make_move(game, promotion_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify promotion: g2 empty, queen on g1
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 2, 'g'));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, 1, 'g', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK));

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CMovesOnBoardTest, PawnPromotionCapture) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
        "2q5/1P2k3/8/8/8/2RQ4/1K4p1/8 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
    ASSERT_GAME_NOT_NULL(game);

    // Verify initial state: white pawn on b7, black queen on c8
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 7, 'b', SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE));
    EXPECT_TRUE(is_piece_at_square(current_stage(game).board, 8, 'c', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK));

    simple_chess_piece_move_t promotion_move = create_promotion_move(SIMPLE_CHESS_COLOR_WHITE, 7, 'b', 8, 'c', SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    simple_chess_game_t* after_move = simple_chess_make_move(game, promotion_move, NULL);
    ASSERT_GAME_NOT_NULL(after_move);

    // Verify promotion with capture: b7 empty, white queen on c8
    EXPECT_TRUE(is_square_empty(current_stage(after_move).board, 7, 'b'));
    EXPECT_TRUE(is_piece_at_square(current_stage(after_move).board, 8, 'c', SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_WHITE));

    simple_chess_destroy_game(game);
    simple_chess_destroy_game(after_move);
}

TEST(CMovesOnBoardTest, KnightNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1",
        SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 5, 'c', 3, 'b');
}

TEST(CMovesOnBoardTest, KnightCapture) {
    test_regular_capture_move(
        "8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1",
        SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_BLACK, 5, 'c', 3, 'd');
}

TEST(CMovesOnBoardTest, BishopNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 3, 'd', 1, 'f');
}

TEST(CMovesOnBoardTest, BishopCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_BISHOP, SIMPLE_CHESS_COLOR_WHITE, 3, 'd', 6, 'g');
}

TEST(CMovesOnBoardTest, RookNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 3, 'c', 4, 'c');
}

TEST(CMovesOnBoardTest, RookCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_ROOK, SIMPLE_CHESS_COLOR_WHITE, 3, 'c', 5, 'c');
}

TEST(CMovesOnBoardTest, QueenNoCapture) {
    test_regular_non_capture_move(
        "8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 5, 'c', 2, 'f');
}

TEST(CMovesOnBoardTest, QueenCapture) {
    test_regular_capture_move(
        "8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_QUEEN, SIMPLE_CHESS_COLOR_BLACK, 5, 'c', 3, 'c');
}

TEST(CMovesOnBoardTest, KingNoCapture) {
    test_regular_non_capture_move(
        "2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'c', 8, 'b');
}

TEST(CMovesOnBoardTest, KingCapture) {
    test_regular_capture_move(
        "2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1",
        SIMPLE_CHESS_PIECE_TYPE_KING, SIMPLE_CHESS_COLOR_BLACK, 8, 'c', 7, 'b');
}

TEST(CMovesOnBoardTest, KingsideCastlingWhite) {
    test_castling(
        "r2qkbnr/ppp2ppp/2np4/1B2p3/6b1/4PN2/PPPP1PPP/RNBQK2R w KQkq - 0 1",
        SIMPLE_CHESS_COLOR_WHITE, 'e', 'g', 'h', 'f');
}

TEST(CMovesOnBoardTest, QueensideCastlingWhite) {
    test_castling(
        "r2qkbnr/ppp2ppp/2np4/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR w KQkq - 0 1",
        SIMPLE_CHESS_COLOR_WHITE, 'e', 'c', 'a', 'd');
}

TEST(CMovesOnBoardTest, KingsideCastlingBlack) {
    test_castling(
        "rn1qk2r/ppp2ppp/3p1n2/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1",
        SIMPLE_CHESS_COLOR_BLACK, 'e', 'g', 'h', 'f');
}

TEST(CMovesOnBoardTest, QueensideCastlingBlack) {
    test_castling(
        "r3kbnr/ppp2ppp/2np4/4p1q1/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1",
        SIMPLE_CHESS_COLOR_BLACK, 'e', 'c', 'a', 'd');
}

// The knight's square is between the king and the rook, so it has to be
// vacant for the rook to pass over it, even though the king never lands on
// it and it may be under attack.
TEST(CMovesOnBoardTest, QueensideCastlingBlockedByKnightOnBFile) {
    EXPECT_FALSE(can_castle("r3k2r/8/8/8/8/8/8/RN2K2R w KQkq - 0 1", 'c'));
    EXPECT_FALSE(can_castle("rn2k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 'c'));

    // With the b-file clear it is available again, which confirms nothing
    // else in these positions is preventing it.
    EXPECT_TRUE(can_castle("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 'c'));
    EXPECT_TRUE(can_castle("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1", 'c'));
}

// A pawn guards the squares it captures towards, whether or not there is
// anything on them to capture. The squares a castling king crosses are empty
// by definition, so this is exactly the case where a pawn's threat has to be
// noticed.
TEST(CMovesOnBoardTest, CastlingThroughSquareAttackedByPawn) {
    // The black pawn on e2 attacks d1 and f1, so the king may cross neither.
    EXPECT_FALSE(can_castle("4k3/8/8/8/8/8/4p3/R3K2R w KQ - 0 1", 'g'));
    EXPECT_FALSE(can_castle("4k3/8/8/8/8/8/4p3/R3K2R w KQ - 0 1", 'c'));

    // Likewise the white pawn on e7 against black.
    EXPECT_FALSE(can_castle("r3k2r/4P3/8/8/8/8/8/4K3 b kq - 0 1", 'g'));
    EXPECT_FALSE(can_castle("r3k2r/4P3/8/8/8/8/8/4K3 b kq - 0 1", 'c'));

    // Without the pawns both castlings are available, which confirms nothing
    // else in these positions is preventing them.
    EXPECT_TRUE(can_castle("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 'g'));
    EXPECT_TRUE(can_castle("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1", 'c'));
}
