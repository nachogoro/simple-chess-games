#include "TestUtils.h"

// A game's history is a vector in C++ and a size with two indexed accessors
// here, so the indexing is this API's own and is what these check.

namespace
{
	const simple_chess_piece_move_t OPENING[] = {
		create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_WHITE, 2, 'e', 4, 'e'),
		create_move(SIMPLE_CHESS_PIECE_TYPE_PAWN, SIMPLE_CHESS_COLOR_BLACK, 7, 'e', 5, 'e'),
		create_move(SIMPLE_CHESS_PIECE_TYPE_KNIGHT, SIMPLE_CHESS_COLOR_WHITE, 1, 'g', 3, 'f'),
	};

	const uint16_t OPENING_LENGTH
		= static_cast<uint16_t>(sizeof(OPENING) / sizeof(OPENING[0]));
}

TEST(CHistoryTest, HistoryStageIsWhereTheMoveWasPlayedFrom) {
	// Each position is kept as the game reaches it, so that the history can
	// be compared against the positions themselves rather than against FEN
	// strings written out by hand.
	simple_chess_game_t* asReached[OPENING_LENGTH + 1] = {};

	asReached[0] = simple_chess_create_new_game(
			SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
	ASSERT_GAME_NOT_NULL(asReached[0]);

	for (uint16_t i = 0; i < OPENING_LENGTH; ++i) {
		asReached[i + 1] = simple_chess_make_move(asReached[i], OPENING[i], NULL);
		ASSERT_GAME_NOT_NULL(asReached[i + 1]);
		EXPECT_EQ(simple_chess_game_history_size(asReached[i + 1]), i + 1);
	}

	simple_chess_game_t* played = asReached[OPENING_LENGTH];

	for (uint16_t i = 0; i < OPENING_LENGTH; ++i) {
		simple_chess_game_stage_t stage = {};
		ASSERT_TRUE(simple_chess_game_history_stage(played, i, &stage))
			<< "at index " << i;

		// The stage recorded against a move is the one it was played from,
		// which is the position the game stood in before it
		EXPECT_STREQ(stage.fen, current_stage(asReached[i]).fen)
			<< "at index " << i;

		// And it is the move played from it
		EXPECT_TRUE(simple_chess_piece_move_equals(
					history_move(played, i).move, OPENING[i]))
			<< "at index " << i;
	}

	// The last stage of the history is not the current one: no move has been
	// played from that.
	simple_chess_game_stage_t last = {};
	ASSERT_TRUE(simple_chess_game_history_stage(played, OPENING_LENGTH - 1, &last));
	EXPECT_STRNE(last.fen, current_stage(played).fen);

	for (simple_chess_game_t* game : asReached) {
		simple_chess_destroy_game(game);
	}
}

TEST(CHistoryTest, ANewGameHasNoHistory) {
	simple_chess_game_t* game = simple_chess_create_new_game(
			SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
	ASSERT_GAME_NOT_NULL(game);

	EXPECT_EQ(simple_chess_game_history_size(game), 0);

	simple_chess_game_stage_t stage = {};
	EXPECT_FALSE(simple_chess_game_history_stage(game, 0, &stage));

	simple_chess_destroy_game(game);
}

TEST(CHistoryTest, HistoryStageRejectsWhatItCannotAnswer) {
	simple_chess_game_t* game = simple_chess_create_new_game(
			SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, NULL);
	ASSERT_GAME_NOT_NULL(game);

	simple_chess_game_t* played = simple_chess_make_move(game, OPENING[0], NULL);
	ASSERT_GAME_NOT_NULL(played);
	ASSERT_EQ(simple_chess_game_history_size(played), 1);

	simple_chess_game_stage_t stage = {};

	// One past the end, and far past it
	EXPECT_FALSE(simple_chess_game_history_stage(played, 1, &stage));
	EXPECT_FALSE(simple_chess_game_history_stage(played, 1000, &stage));

	// Neither pointer may be NULL
	EXPECT_FALSE(simple_chess_game_history_stage(NULL, 0, &stage));
	EXPECT_FALSE(simple_chess_game_history_stage(played, 0, NULL));

	// The index which is in range still works, so the rejections above are
	// not simply the accessor refusing everything
	EXPECT_TRUE(simple_chess_game_history_stage(played, 0, &stage));

	simple_chess_destroy_game(game);
	simple_chess_destroy_game(played);
}
