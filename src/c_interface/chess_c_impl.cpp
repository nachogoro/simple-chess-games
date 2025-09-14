#include <c/simplechess/chess.h>
#include "conversion_utils.h"
#include <cpp/simplechess/GameManager.h>
#include <cpp/simplechess/Exceptions.h>
#include <memory>
#include <cstring>

using namespace simplechess;
using namespace conversion_utils;

// ===== CORE API FUNCTIONS =====

chess_game_manager_t chess_create_manager(void) {
    try {
        return new chess_game_manager();
    } catch (...) {
        return nullptr;
    }
}

void chess_destroy_manager(chess_game_manager_t manager) {
    if (manager) {
        delete manager;
    }
}

bool chess_create_new_game(chess_game_manager_t manager, chess_game_t* game_out) {
    if (!manager || !game_out) {
        return false;
    }

    try {
        Game cpp_game = manager->manager->createNewGame();
        *game_out = cpp_to_c_game(cpp_game);
        return true;
    } catch (...) {
        return false;
    }
}

bool chess_create_game_from_fen(chess_game_manager_t manager, const char* fen, chess_game_t* game_out) {
    if (!manager || !fen || !game_out) {
        return false;
    }

    try {
        Game cpp_game = manager->manager->createGameFromFen(std::string(fen));
        *game_out = cpp_to_c_game(cpp_game);
        return true;
    } catch (...) {
        return false;
    }
}

bool chess_make_move(chess_game_manager_t manager,
                     const chess_game_t* current_game,
                     const chess_move_t* move,
                     bool offer_draw,
                     chess_game_t* result_game) {
    if (!manager || !current_game || !move || !result_game) {
        return false;
    }

    try {
        // Reconstruct C++ Game from C game history
        Game cpp_game = c_to_cpp_game(manager, *current_game);

        // Convert C move to C++ move
        PieceMove cpp_move = c_to_cpp_move(*move);

        // Make the move using C++ GameManager
        Game new_cpp_game = manager->manager->makeMove(cpp_game, cpp_move, offer_draw);

        // Convert result back to C format
        *result_game = cpp_to_c_game(new_cpp_game);

        return true;
    } catch (...) {
        return false;
    }
}

bool chess_claim_draw(chess_game_manager_t manager,
                      const chess_game_t* current_game,
                      chess_game_t* result_game) {
    if (!manager || !current_game || !result_game) {
        return false;
    }

    try {
        // Reconstruct C++ Game from C game history
        Game cpp_game = c_to_cpp_game(manager, *current_game);

        // Claim draw using C++ GameManager
        Game new_cpp_game = manager->manager->claimDraw(cpp_game);

        // Convert result back to C format
        *result_game = cpp_to_c_game(new_cpp_game);

        return true;
    } catch (...) {
        return false;
    }
}

bool chess_resign(chess_game_manager_t manager,
                  const chess_game_t* current_game,
                  chess_color_t resigning_player,
                  chess_game_t* result_game) {
    if (!manager || !current_game || !result_game) {
        return false;
    }

    try {
        // Reconstruct C++ Game from C game history
        Game cpp_game = c_to_cpp_game(manager, *current_game);

        // Convert C color to C++ color
        Color cpp_color = c_to_cpp_color(resigning_player);

        // Resign using C++ GameManager
        Game new_cpp_game = manager->manager->resign(cpp_game, cpp_color);

        // Convert result back to C format
        *result_game = cpp_to_c_game(new_cpp_game);

        return true;
    } catch (...) {
        return false;
    }
}

// ===== MEMORY MANAGEMENT =====

void chess_destroy_game(chess_game_t* game) {
    if (!game) return;
    free_c_game_memory(game);
}


// ===== UTILITY FUNCTIONS =====

chess_square_t chess_square_from_string(const char* square_str) {
    chess_square_t result = {0, 'a'};
    if (!square_str || strlen(square_str) < 2) {
        return result;
    }

    try {
        Square cpp_square = Square::fromString(std::string(square_str));
        result = cpp_to_c_square(cpp_square);
    } catch (...) {
        // Return default invalid square
    }

    return result;
}

void chess_square_to_string(chess_square_t square, char* out_str) {
    if (!out_str) return;

    try {
        Square cpp_square = c_to_cpp_square(square);
        std::string str = cpp_square.toString();
        strncpy(out_str, str.c_str(), 2);
        out_str[2] = '\0';
    } catch (...) {
        strncpy(out_str, "--", 3);
    }
}

bool chess_square_is_valid(chess_square_t square) {
    try {
        c_to_cpp_square(square);
        return true;
    } catch (...) {
        return false;
    }
}

int chess_square_to_index(chess_square_t square) {
    if (!chess_square_is_valid(square)) {
        return -1;
    }
    return (square.rank - 1) * 8 + (square.file - 'a');
}

chess_square_t chess_square_from_index(int index) {
    chess_square_t result = {0, 'a'};
    if (index < 0 || index >= 64) {
        return result;
    }

    result.rank = (index / 8) + 1;
    result.file = (index % 8) + 'a';
    return result;
}

bool chess_move_is_valid_format(const chess_move_t* move) {
    if (!move) return false;

    return chess_square_is_valid(move->from) &&
           chess_square_is_valid(move->to);
}

void chess_move_to_string(const chess_move_t* move, char* out_str) {
    if (!move || !out_str) return;

    char from_str[3], to_str[3];
    chess_square_to_string(move->from, from_str);
    chess_square_to_string(move->to, to_str);

    if (move->is_promotion) {
        char promotion_char = 'q'; // Default to queen
        switch (move->promotion) {
            case CHESS_PIECE_QUEEN:  promotion_char = 'q'; break;
            case CHESS_PIECE_ROOK:   promotion_char = 'r'; break;
            case CHESS_PIECE_BISHOP: promotion_char = 'b'; break;
            case CHESS_PIECE_KNIGHT: promotion_char = 'n'; break;
            default: break;
        }
        snprintf(out_str, 6, "%s%s%c", from_str, to_str, promotion_char);
    } else {
        snprintf(out_str, 5, "%s%s", from_str, to_str);
    }
}

chess_piece_t chess_get_piece_at(const chess_position_t* position, chess_square_t square) {
    chess_piece_t result = {CHESS_PIECE_PAWN, CHESS_COLOR_WHITE}; // Default

    if (!position || !chess_square_is_valid(square)) {
        return result;
    }

    int index = chess_square_to_index(square);
    if (index >= 0 && index < CHESS_BOARD_SIZE && position->occupied[index]) {
        result = position->pieces[index];
    }

    return result;
}

bool chess_is_square_occupied(const chess_position_t* position, chess_square_t square) {
    if (!position || !chess_square_is_valid(square)) {
        return false;
    }

    int index = chess_square_to_index(square);
    return (index >= 0 && index < CHESS_BOARD_SIZE && position->occupied[index]);
}

chess_move_list_t chess_get_moves_for_piece(const chess_position_t* position, chess_square_t square) {
    chess_move_list_t result = {{}, 0};

    if (!position || !chess_square_is_valid(square)) {
        return result;
    }

    // Check if there's a piece on the square
    int index = chess_square_to_index(square);
    if (index < 0 || index >= CHESS_BOARD_SIZE || !position->occupied[index]) {
        return result;
    }

    try {
        // Create a temporary game manager and reconstruct game from position
        auto temp_manager = std::make_unique<GameManager>();

        // For now, we need to reconstruct the game from the position
        // This is a limitation - we'd need a way to create a game from a position directly
        // As a simplified approach, we'll return an empty list since this requires
        // the full game context which isn't available from just the position

        // TODO: This could be improved by adding a method to create a Game from a position
        // or by storing additional context in chess_position_t

        return result;
    } catch (...) {
        return result;
    }
}

chess_move_list_t chess_get_all_moves_(const chess_position_t* position) {
    chess_move_list_t result = {{}, 0};

    if (!position) {
        return result;
    }

    // This would require reconstructing the full game state from the position
    // For now, return empty list as this requires complex position analysis
    // TODO: Implement position-based move generation

    return result;
}

bool chess_get_fen_for_position(const chess_position_t* position, char* out_buffer) {
    if (!position || !out_buffer) {
        return false;
    }

    // This would require constructing FEN from the chess_position_t
    // For now, return false as this requires complex FEN generation
    // TODO: Implement position-to-FEN conversion

    return false;
}
