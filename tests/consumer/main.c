#include <simplechess-c/simplechess.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
	simple_chess_error_t error = SIMPLE_CHESS_OK;
	simple_chess_game_t* game;
	simple_chess_game_t* played;
	simple_chess_piece_move_t move;
	simple_chess_played_move_t last;
	simple_chess_square_t src;
	simple_chess_square_t dst;

	game = simple_chess_create_new_game(
			SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC, &error);
	if (game == NULL)
	{
		printf("could not create a game: %s\n",
				simple_chess_error_string(error));
		return 1;
	}

	if (!simple_chess_square_from_string("g1", &src)
			|| !simple_chess_square_from_string("f3", &dst))
	{
		return 1;
	}

	if (!simple_chess_find_move(
				game, src, dst, false, SIMPLE_CHESS_PIECE_TYPE_PAWN, &move))
	{
		puts("no such move");
		return 1;
	}

	played = simple_chess_make_move(game, move, &error);
	if (played == NULL)
	{
		printf("could not play the move: %s\n",
				simple_chess_error_string(error));
		return 1;
	}

	if (!simple_chess_game_history_move(
				played, simple_chess_game_history_size(played) - 1, &last))
	{
		return 1;
	}

	if (strcmp(last.in_algebraic_notation, "Nf3") != 0)
	{
		puts("unexpected move");
		return 1;
	}

	printf("C consumer: %s, %u moves available\n",
			last.in_algebraic_notation,
			(unsigned) simple_chess_game_available_move_count(played));

	simple_chess_destroy_game(played);
	simple_chess_destroy_game(game);
	return 0;
}
