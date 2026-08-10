/*
 * The rest of the C test suite is written in C++, because that is what
 * GoogleTest needs, which means it never checks the one thing the C headers
 * exist for: being usable from C. This file is the only translation unit in
 * the project compiled by a C compiler, and it is compiled with the same
 * warnings-as-errors as everything else.
 *
 * It is deliberately not a test of behaviour. It has to name every public
 * type and call every public function, so that anything which stops being
 * valid C - a missing typedef, a parameterless function declared without
 * void, a C++ keyword used as an identifier - fails the build.
 */

#include <simplechess-c/simplechess.h>

#include <stddef.h>

/* Every public type, spelled without the struct/enum keyword C would
 * otherwise require. */
static simple_chess_color_t              g_color;
static simple_chess_square_t             g_square;
static simple_chess_piece_type_t         g_piece_type;
static simple_chess_piece_t              g_piece;
static simple_chess_piece_move_t         g_piece_move;
static simple_chess_check_type_t         g_check_type;
static simple_chess_played_move_t        g_played_move;
static simple_chess_castling_right_t     g_castling_right;
static simple_chess_board_t              g_board;
static simple_chess_game_stage_t         g_game_stage;
static simple_chess_game_state_t         g_game_state;
static simple_chess_draw_reason_t        g_draw_reason;
static simple_chess_game_history_entry_t g_history_entry;
static simple_chess_draw_enforcement_t   g_draw_enforcement;
static simple_chess_square_content_t     g_square_content;

int simple_chess_c_header_compile_check(void);

int simple_chess_c_header_compile_check(void)
{
	simple_chess_game_t* game;
	simple_chess_game_t* next;

	g_color = SIMPLE_CHESS_COLOR_WHITE;
	g_piece_type = SIMPLE_CHESS_PIECE_TYPE_PAWN;
	g_check_type = SIMPLE_CHESS_CHECK_TYPE_NONE;
	g_castling_right = SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE;
	g_game_state = SIMPLE_CHESS_GAME_STATE_PLAYING;
	g_draw_reason = SIMPLE_CHESS_DRAW_REASON_STALEMATE;
	g_draw_enforcement = SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC;

	g_square = simple_chess_square_from_index(0);
	g_piece.type = g_piece_type;
	g_piece.color = g_color;
	g_piece_move.piece = g_piece;
	g_piece_move.src = g_square;
	g_piece_move.dst = simple_chess_square_from_index(1);
	g_piece_move.is_promotion = false;
	g_piece_move.promoted_to = g_piece_type;
	g_played_move.move = g_piece_move;
	g_board.squares[0] = SIMPLE_CHESS_SQUARE_WHITE_ROOK;
	g_game_stage.active_color = g_color;
	g_history_entry.played_move = g_played_move;

	game = simple_chess_create_new_game();
	if (game == NULL)
	{
		return 1;
	}

	next = simple_chess_create_new_game_ex(g_draw_enforcement);
	destroy_game(next);

	next = simple_chess_create_game_from_fen("8/8/8/8/8/8/8/K6k w - - 0 1");
	destroy_game(next);

	next = simple_chess_create_game_from_fen_ex(
			"8/8/8/8/8/8/8/K6k w - - 0 1", g_draw_enforcement);
	destroy_game(next);

	if (game->available_move_count > 0)
	{
		next = simple_chess_make_move(game, game->available_moves[0]);
		destroy_game(next);

		next = simple_chess_make_move_with_draw_offer(
				game, game->available_moves[0], true);
		destroy_game(next);
	}

	next = simple_chess_claim_draw(game);
	destroy_game(next);

	next = simple_chess_resign(game, SIMPLE_CHESS_COLOR_WHITE);
	destroy_game(next);

	(void) simple_chess_index_from_square(g_square);

	g_square_content = simple_chess_square_content_from_piece(g_piece);
	(void) simple_chess_square_content_piece(g_square_content, &g_piece);

	destroy_game(game);
	return 0;
}
