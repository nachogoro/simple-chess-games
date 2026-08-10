#include <simplechess-c/simplechess.h>
#include <simplechess/SimpleChess.h>

#include "conversion_utils.h"

#include <new>
#include <utility>

using namespace conversion_utils;

/**
 * A game handle holds the C++ game itself, rather than a description of it
 * which every call would have to turn back into one.
 */
struct simple_chess_game {
	explicit simple_chess_game(simplechess::Game g)
		: game(std::move(g))
	{
	}

	simplechess::Game game;
};

namespace {
	/**
	 * Wraps a game in a handle, or returns NULL if there is no memory for
	 * one. Every factory ends here.
	 */
	simple_chess_game_t* handle(simplechess::Game game) {
		return new (std::nothrow) simple_chess_game(std::move(game));
	}
}

simple_chess_game_t* simple_chess_create_new_game(simple_chess_draw_enforcement_t draw_enforcement) {
	try {
		return handle(simplechess::createNewGame(
					cpp_draw_enforcement(draw_enforcement)));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_create_game_from_fen(const char* fen, simple_chess_draw_enforcement_t draw_enforcement) {
	if (!fen) return nullptr;

	try {
		return handle(simplechess::createGameFromFen(
					fen, cpp_draw_enforcement(draw_enforcement)));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_make_move(const simple_chess_game_t* game, simple_chess_piece_move_t move) {
	if (!game) return nullptr;

	return simple_chess_make_move_with_draw_offer(game, move, false);
}

simple_chess_game_t* simple_chess_make_move_with_draw_offer(const simple_chess_game_t* game, simple_chess_piece_move_t move, bool offer_draw) {
	if (!game) return nullptr;

	try {
		return handle(simplechess::makeMove(
					game->game, cpp_piece_move(move), offer_draw));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_claim_draw(const simple_chess_game_t* game) {
	if (!game) return nullptr;

	try {
		return handle(simplechess::claimDraw(game->game));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_resign(const simple_chess_game_t* game, simple_chess_color_t resigner) {
	if (!game) return nullptr;

	try {
		return handle(simplechess::resign(game->game, cpp_color(resigner)));
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_copy_game(const simple_chess_game_t* game) {
	if (!game) return nullptr;

	try {
		return handle(game->game);
	} catch (...) {
		return nullptr;
	}
}

simple_chess_game_state_t simple_chess_game_state(const simple_chess_game_t* game) {
	return c_game_state(game->game.gameState());
}

simple_chess_color_t simple_chess_game_active_color(const simple_chess_game_t* game) {
	return c_color(game->game.activeColor());
}

simple_chess_draw_enforcement_t simple_chess_game_draw_enforcement(
		const simple_chess_game_t* game) {
	return c_draw_enforcement(game->game.drawEnforcement());
}

bool simple_chess_game_draw_reason(
		const simple_chess_game_t* game,
		simple_chess_draw_reason_t* const out) {
	if (!game || !out) return false;

	const std::optional<simplechess::DrawReason>& reason = game->game.drawReason();
	if (!reason) return false;

	*out = c_draw_reason(*reason);
	return true;
}

bool simple_chess_game_reason_to_claim_draw(
		const simple_chess_game_t* game,
		simple_chess_draw_reason_t* const out) {
	if (!game || !out) return false;

	const std::optional<simplechess::DrawReason>& reason
		= game->game.reasonToClaimDraw();
	if (!reason) return false;

	*out = c_draw_reason(*reason);
	return true;
}

bool simple_chess_game_current_stage(
		const simple_chess_game_t* game,
		simple_chess_game_stage_t* const out) {
	if (!game || !out) return false;

	*out = c_game_stage(game->game.currentStage());
	return true;
}

uint16_t simple_chess_game_history_size(const simple_chess_game_t* game) {
	return static_cast<uint16_t>(game->game.history().size());
}

bool simple_chess_game_history_stage(
		const simple_chess_game_t* game,
		const uint16_t index,
		simple_chess_game_stage_t* const out) {
	if (!game || !out || index >= game->game.history().size()) return false;

	*out = c_game_stage(game->game.history()[index].stage);
	return true;
}

bool simple_chess_game_history_move(
		const simple_chess_game_t* game,
		const uint16_t index,
		simple_chess_played_move_t* const out) {
	if (!game || !out || index >= game->game.history().size()) return false;

	*out = c_played_move(game->game.history()[index].move);
	return true;
}

uint16_t simple_chess_game_available_move_count(const simple_chess_game_t* game) {
	return static_cast<uint16_t>(game->game.allAvailableMoves().size());
}

bool simple_chess_game_available_move(
		const simple_chess_game_t* game,
		const uint16_t index,
		simple_chess_piece_move_t* const out) {
	if (!game || !out || index >= game->game.allAvailableMoves().size())
		return false;

	*out = c_piece_move(game->game.allAvailableMoves()[index]);
	return true;
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
	delete game;
}

