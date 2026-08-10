#include <simplechess-c/simplechess.h>
#include <simplechess/SimpleChess.h>

#include "conversion_utils.h"

#include <new>
#include <stdexcept>
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
	void report(simple_chess_error_t* const error, const simple_chess_error_t value) {
		if (error) *error = value;
	}

	/**
	 * Wraps a game in a handle. Every factory ends here.
	 */
	simple_chess_game_t* handle(
			simplechess::Game game,
			simple_chess_error_t* const error) {
		simple_chess_game_t* const result
			= new (std::nothrow) simple_chess_game(std::move(game));

		report(error, result
				? SIMPLE_CHESS_OK
				: SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return result;
	}

	/**
	 * Why an operation on \p game could not be carried out.
	 *
	 * The C++ API reports all three of these as one IllegalStateException,
	 * so they are told apart by asking the game what it would have
	 * objected to. Only reached once an operation has already failed.
	 */
	simple_chess_error_t reasonMoveFailed(const simple_chess_game_t* game) {
		return game->game.gameState() != simplechess::GameState::Playing
			? SIMPLE_CHESS_ERROR_GAME_OVER
			: SIMPLE_CHESS_ERROR_ILLEGAL_MOVE;
	}
}

simple_chess_game_t* simple_chess_create_new_game(
		simple_chess_draw_enforcement_t draw_enforcement,
		simple_chess_error_t* error) {
	try {
		return handle(
				simplechess::createNewGame(cpp_draw_enforcement(draw_enforcement)),
				error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_create_game_from_fen(
		const char* fen,
		simple_chess_draw_enforcement_t draw_enforcement,
		simple_chess_error_t* error) {
	if (!fen) {
		report(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
		return nullptr;
	}

	try {
		return handle(
				simplechess::createGameFromFen(
					fen, cpp_draw_enforcement(draw_enforcement)),
				error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return nullptr;
	} catch (const std::invalid_argument&) {
		report(error, SIMPLE_CHESS_ERROR_INVALID_FEN);
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_make_move(
		const simple_chess_game_t* game,
		simple_chess_piece_move_t move,
		simple_chess_error_t* error) {
	return simple_chess_make_move_with_draw_offer(game, move, false, error);
}

simple_chess_game_t* simple_chess_make_move_with_draw_offer(
		const simple_chess_game_t* game,
		simple_chess_piece_move_t move,
		bool offer_draw,
		simple_chess_error_t* error) {
	if (!game) {
		report(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
		return nullptr;
	}

	try {
		return handle(
				simplechess::makeMove(
					game->game, cpp_piece_move(move), offer_draw),
				error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return nullptr;
	} catch (const std::invalid_argument&) {
		// The move does not describe anything playable at all, such as a
		// promotion to a king, so no position could accept it.
		report(error, SIMPLE_CHESS_ERROR_ILLEGAL_MOVE);
		return nullptr;
	} catch (const simplechess::IllegalStateException&) {
		report(error, reasonMoveFailed(game));
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_claim_draw(
		const simple_chess_game_t* game,
		simple_chess_error_t* error) {
	if (!game) {
		report(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
		return nullptr;
	}

	try {
		return handle(simplechess::claimDraw(game->game), error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return nullptr;
	} catch (const simplechess::IllegalStateException&) {
		report(error,
				game->game.gameState() != simplechess::GameState::Playing
					? SIMPLE_CHESS_ERROR_GAME_OVER
					: SIMPLE_CHESS_ERROR_DRAW_NOT_CLAIMABLE);
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_resign(
		const simple_chess_game_t* game,
		simple_chess_color_t resigner,
		simple_chess_error_t* error) {
	if (!game) {
		report(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
		return nullptr;
	}

	try {
		return handle(
				simplechess::resign(game->game, cpp_color(resigner)), error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
		return nullptr;
	} catch (const simplechess::IllegalStateException&) {
		report(error, SIMPLE_CHESS_ERROR_GAME_OVER);
		return nullptr;
	}
}

simple_chess_game_t* simple_chess_copy_game(
		const simple_chess_game_t* game,
		simple_chess_error_t* error) {
	if (!game) {
		report(error, SIMPLE_CHESS_ERROR_NULL_ARGUMENT);
		return nullptr;
	}

	try {
		return handle(game->game, error);
	} catch (const std::bad_alloc&) {
		report(error, SIMPLE_CHESS_ERROR_OUT_OF_MEMORY);
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


const char* simple_chess_error_string(const simple_chess_error_t error) {
	switch (error) {
		case SIMPLE_CHESS_OK:
			return "no error";
		case SIMPLE_CHESS_ERROR_NULL_ARGUMENT:
			return "a required argument was NULL";
		case SIMPLE_CHESS_ERROR_INVALID_FEN:
			return "the string is not a valid FEN description of a position";
		case SIMPLE_CHESS_ERROR_ILLEGAL_MOVE:
			return "the move is not available to the player to move";
		case SIMPLE_CHESS_ERROR_GAME_OVER:
			return "the game has already finished";
		case SIMPLE_CHESS_ERROR_DRAW_NOT_CLAIMABLE:
			return "no draw can be claimed in this position";
		case SIMPLE_CHESS_ERROR_OUT_OF_MEMORY:
			return "out of memory";
	}

	return "unknown error";
}
