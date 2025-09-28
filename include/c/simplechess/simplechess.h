#ifndef SIMPLE_CHESS_C_H_58ADD4DC_9C5F_11F0_B0FF_1BA779F2B9FB
#define SIMPLE_CHESS_C_H_58ADD4DC_9C5F_11F0_B0FF_1BA779F2B9FB

#include "simplechess_types.h"

game_t* simple_chess_create_new_game();
game_t* simple_chess_create_game_from_fen(const char* fen);
game_t* simple_chess_make_move(const game_t* game, piece_move_t move);
game_t* simple_chess_make_move(const game_t* game, piece_move_t move, bool offer_draw);
game_t* simple_chess_claim_draw(const game_t* game);
game_t* simple_chess_resign(const game_t* game, color_t resigner);

square_t simple_chess_square_from_index(uint8_t index);
uint8_t simple_chess_index_from_square(square_t square);

void destroy_game(game_t* game);

#endif
