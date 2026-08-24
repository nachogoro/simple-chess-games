#include "TestUtils.h"

TEST(CValueHelpersTest, SquareIndexRoundTrip) {
    for (uint8_t index = 0; index < 64; ++index) {
        simple_chess_square_t square;
        ASSERT_TRUE(simple_chess_square_from_index(index, &square));

        uint8_t back = 0;
        ASSERT_TRUE(simple_chess_index_from_square(square, &back));
        EXPECT_EQ(back, index);
    }

    // a1 is index 0 and h8 is index 63.
    simple_chess_square_t corner;
    ASSERT_TRUE(simple_chess_square_from_index(0, &corner));
    EXPECT_EQ(corner.rank, 1);
    EXPECT_EQ(corner.file, 'a');

    ASSERT_TRUE(simple_chess_square_from_index(63, &corner));
    EXPECT_EQ(corner.rank, 8);
    EXPECT_EQ(corner.file, 'h');
}

TEST(CValueHelpersTest, SquaresOffTheBoardAreRejected) {
    // Out of range used to be undefined behaviour rather than a failure.
    simple_chess_square_t square;
    EXPECT_FALSE(simple_chess_square_from_index(64, &square));
    EXPECT_FALSE(simple_chess_square_from_index(255, &square));

    uint8_t index = 0;
    EXPECT_FALSE(simple_chess_index_from_square({0, 'a'}, &index));
    EXPECT_FALSE(simple_chess_index_from_square({9, 'a'}, &index));
    EXPECT_FALSE(simple_chess_index_from_square({1, 'i'}, &index));

    EXPECT_TRUE(simple_chess_square_is_valid({1, 'a'}));
    EXPECT_TRUE(simple_chess_square_is_valid({8, 'h'}));
    EXPECT_FALSE(simple_chess_square_is_valid({0, 'a'}));
    EXPECT_FALSE(simple_chess_square_is_valid({1, 'z'}));
}

TEST(CValueHelpersTest, SquareStrings) {
    simple_chess_square_t square;
    ASSERT_TRUE(simple_chess_square_from_string("e4", &square));
    EXPECT_EQ(square.rank, 4);
    EXPECT_EQ(square.file, 'e');

    // Case does not matter, matching the C++ Square::fromString.
    ASSERT_TRUE(simple_chess_square_from_string("E4", &square));
    EXPECT_EQ(square.rank, 4);
    EXPECT_EQ(square.file, 'e');

    EXPECT_FALSE(simple_chess_square_from_string("e9", &square));
    EXPECT_FALSE(simple_chess_square_from_string("", &square));
    EXPECT_FALSE(simple_chess_square_from_string(nullptr, &square));

    char text[3] = {'x', 'x', 'x'};
    simple_chess_square_to_string({4, 'e'}, text);
    EXPECT_STREQ(text, "e4");

    simple_chess_square_to_string({9, 'e'}, text);
    EXPECT_STREQ(text, "");
}

TEST(CValueHelpersTest, SquareColors) {
    EXPECT_EQ(simple_chess_square_color({1, 'a'}), SIMPLE_CHESS_COLOR_BLACK);
    EXPECT_EQ(simple_chess_square_color({1, 'h'}), SIMPLE_CHESS_COLOR_WHITE);
    EXPECT_EQ(simple_chess_square_color({8, 'a'}), SIMPLE_CHESS_COLOR_WHITE);
}

TEST(CValueHelpersTest, MoveEquality) {
    const simple_chess_piece_move_t knight = create_move(
            SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE,
            1, 'g', 3, 'f');

    EXPECT_TRUE(simple_chess_piece_move_equals(knight, knight));
    EXPECT_FALSE(simple_chess_piece_move_equals(
                knight,
                create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT,
                    SIMPLE_CHESS_COLOR_WHITE, 1, 'b', 3, 'c')));

    // Where no pawn is promoted, promoted_to carries no meaning and must not
    // be compared: the tests fill it with a dummy value.
    simple_chess_piece_move_t otherDummy = knight;
    otherDummy.promoted_to = SIMPLE_CHESS_PIECE_TYPE_QUEEN;
    EXPECT_TRUE(simple_chess_piece_move_equals(knight, otherDummy));

    const simple_chess_piece_move_t toQueen = create_promotion_move(
            SIMPLE_CHESS_COLOR_WHITE, 7, 'f', 8, 'f',
            SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    const simple_chess_piece_move_t toKnight = create_promotion_move(
            SIMPLE_CHESS_COLOR_WHITE, 7, 'f', 8, 'f',
            SIMPLE_CHESS_PIECE_TYPE_KNIGHT);

    EXPECT_FALSE(simple_chess_piece_move_equals(toQueen, toKnight));
    EXPECT_FALSE(simple_chess_piece_move_equals(toQueen, knight));
}

TEST(CValueHelpersTest, FindMoveBySourceAndDestination) {
    simple_chess_game_t* game = simple_chess_create_new_game(
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, nullptr);
    ASSERT_GAME_NOT_NULL(game);

    // The caller does not have to know which piece stands on g1.
    simple_chess_piece_move_t move;
    ASSERT_TRUE(simple_chess_find_move(
                game, {1, 'g'}, {3, 'f'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move));

    EXPECT_EQ(move.piece.type, SIMPLE_CHESS_PIECE_TYPE_KNIGHT);
    EXPECT_EQ(move.piece.color, SIMPLE_CHESS_COLOR_WHITE);
    EXPECT_FALSE(move.is_promotion);

    // The move it found is one which can actually be played.
    simple_chess_game_t* after = simple_chess_make_move(game, move, nullptr);
    EXPECT_GAME_NOT_NULL(after);
    simple_chess_destroy_game(after);

    // A move which is not legal here is not found, though both squares exist.
    EXPECT_FALSE(simple_chess_find_move(
                game, {2, 'e'}, {5, 'e'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move));

    // Nor is a move of a piece belonging to the player who is not to play.
    EXPECT_FALSE(simple_chess_find_move(
                game, {7, 'e'}, {5, 'e'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move));

    // Nor is a square off the board accepted.
    EXPECT_FALSE(simple_chess_find_move(
                game, {9, 'e'}, {5, 'e'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move));

    simple_chess_destroy_game(game);
}

TEST(CValueHelpersTest, FindMoveDistinguishesPromotions) {
    simple_chess_game_t* game = simple_chess_create_game_from_fen(
            "k7/5P2/8/8/8/8/8/4K3 w - - 0 1",
            SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC,
            nullptr);
    ASSERT_GAME_NOT_NULL(game);

    simple_chess_piece_move_t toQueen;
    simple_chess_piece_move_t toKnight;
    ASSERT_TRUE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                true, SIMPLE_CHESS_PIECE_TYPE_QUEEN, &toQueen));
    ASSERT_TRUE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                true, SIMPLE_CHESS_PIECE_TYPE_KNIGHT, &toKnight));

    EXPECT_EQ(toQueen.promoted_to, SIMPLE_CHESS_PIECE_TYPE_QUEEN);
    EXPECT_EQ(toKnight.promoted_to, SIMPLE_CHESS_PIECE_TYPE_KNIGHT);

    // A pawn reaching the last rank always promotes, so there is no
    // non-promoting move between the same two squares.
    simple_chess_piece_move_t plain;
    EXPECT_FALSE(simple_chess_find_move(
                game, {7, 'f'}, {8, 'f'},
                false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &plain));

    simple_chess_destroy_game(game);
}
