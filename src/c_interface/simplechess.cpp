#include <c/simplechess/simplechess.h>
#include <cpp/simplechess/SimpleChess.h>

#include "conversion_utils.h"

using namespace conversion_utils;

game_t* simple_chess_create_new_game() {
	try {
		return c_game(simplechess::createNewGame());
	} catch (...) {
		return nullptr;
	}
}

game_t* simple_chess_create_game_from_fen(const char* fen) {
	if (!fen) return nullptr;

	try {
		return c_game(simplechess::createGameFromFen(fen));
	} catch (...) {
		return nullptr;
	}
}

game_t* simple_chess_make_move(const game_t* game, piece_move_t move) {
	if (!game) return nullptr;

	try {
		return simple_chess_make_move_with_draw_offer(game, move, false);
	} catch (...) {
		return nullptr;
	}
}

game_t* simple_chess_make_move_with_draw_offer(const game_t* game, piece_move_t move, bool offer_draw) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::makeMove(cppGame, cpp_piece_move(move), offer_draw);
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

game_t* simple_chess_claim_draw(const game_t* game) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::claimDraw(cppGame);
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

game_t* simple_chess_resign(const game_t* game, color_t resigner) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::resign(cppGame, cpp_color(resigner));
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

square_t simple_chess_square_from_index(uint8_t index) {
	uint8_t row = 1 + (index / 8);
	char col = 'a' + (index % 8);
	return {row, col};
}

uint8_t simple_chess_index_from_square(square_t square) {
	uint8_t row = square.rank - 1;
	uint8_t col = square.file - 'a';
	return row * 8 + col;

}

void destroy_game(game_t* game) {
	if (!game) return;

	delete[] game->history;
	delete[] game->available_moves;
	delete game;
}

