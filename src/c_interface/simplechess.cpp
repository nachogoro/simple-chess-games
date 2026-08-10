#include <simplechess-c/simplechess.h>
#include <simplechess/SimpleChess.h>

#include "conversion_utils.h"

using namespace conversion_utils;

simple_chess_game_t* simple_chess_create_new_game(simple_chess_draw_enforcement_t draw_enforcement) {
	try {
		return c_game(simplechess::createNewGame(cpp_draw_enforcement(draw_enforcement)));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_create_game_from_fen(const char* fen, simple_chess_draw_enforcement_t draw_enforcement) {
	if (!fen) return nullptr;

	try {
		return c_game(simplechess::createGameFromFen(fen, cpp_draw_enforcement(draw_enforcement)));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_make_move(const simple_chess_game_t* game, simple_chess_piece_move_t move) {
	if (!game) return nullptr;

	try {
		return simple_chess_make_move_with_draw_offer(game, move, false);
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_make_move_with_draw_offer(const simple_chess_game_t* game, simple_chess_piece_move_t move, bool offer_draw) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::makeMove(cppGame, cpp_piece_move(move), offer_draw);
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_claim_draw(const simple_chess_game_t* game) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::claimDraw(cppGame);
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_resign(const simple_chess_game_t* game, simple_chess_color_t resigner) {
	if (!game) return nullptr;

	try {
		const simplechess::Game cppGame = cpp_game(*game);
		const simplechess::Game updatedGame = simplechess::resign(cppGame, cpp_color(resigner));
		return c_game(updatedGame);
	} catch (...) {
		return nullptr;
	}
}

bool simple_chess_square_content_piece(
		const simple_chess_square_content_t content,
		simple_chess_piece_t* const out) {
	if (content == SIMPLE_CHESS_SQUARE_EMPTY || !out) return false;

	// The two colours are laid out as two consecutive runs of the six piece
	// types, in the order simple_chess_piece_type_t declares them.
	const int offset = content - SIMPLE_CHESS_SQUARE_WHITE_PAWN;
	out->color = (offset < 6)
		? SIMPLE_CHESS_COLOR_WHITE
		: SIMPLE_CHESS_COLOR_BLACK;
	out->type = static_cast<simple_chess_piece_type_t>(offset % 6);
	return true;
}

simple_chess_square_content_t simple_chess_square_content_from_piece(
		const simple_chess_piece_t piece) {
	const int base = (piece.color == SIMPLE_CHESS_COLOR_WHITE)
		? SIMPLE_CHESS_SQUARE_WHITE_PAWN
		: SIMPLE_CHESS_SQUARE_BLACK_PAWN;

	return static_cast<simple_chess_square_content_t>(base + piece.type);
}

simple_chess_square_t simple_chess_square_from_index(uint8_t index) {
	uint8_t row = 1 + (index / 8);
	char col = 'a' + (index % 8);
	return {row, col};
}

uint8_t simple_chess_index_from_square(simple_chess_square_t square) {
	uint8_t row = square.rank - 1;
	uint8_t col = square.file - 'a';
	return row * 8 + col;

}

void simple_chess_destroy_game(simple_chess_game_t* game) {
	if (!game) return;

	delete[] game->history;
	delete[] game->available_moves;
	delete game;
}

