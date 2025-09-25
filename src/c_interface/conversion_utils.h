#ifndef CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19
#define CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19

#include <c/simplechess/chess.h>
#include <cpp/simplechess/Game.h>
#include <cpp/simplechess/Square.h>
#include <cpp/simplechess/Piece.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/Color.h>
#include <cpp/simplechess/GameManager.h>
#include <set>
#include <memory>

// Forward declarations
// The opaque handle chess_game_manager_t is defined in the C header as a pointer to an opaque struct
// We cast GameManager* to/from chess_game_manager_t as needed

namespace conversion_utils {

// C++ to C conversions
chess_color_t cpp_to_c_color(simplechess::Color color);
chess_piece_type_t cpp_to_c_piece_type(simplechess::PieceType type);
chess_game_state_t cpp_to_c_game_state(simplechess::GameState state);
chess_draw_reason_t cpp_to_c_draw_reason(simplechess::DrawReason reason);
chess_check_type_t cpp_to_c_check_type(simplechess::CheckType check_type);
chess_square_t cpp_to_c_square(const simplechess::Square& square);
chess_piece_t cpp_to_c_piece(const simplechess::Piece& piece);
chess_move_t cpp_to_c_move(const simplechess::PieceMove& move);
chess_played_move_t cpp_to_c_played_move(const simplechess::PlayedMove& played_move);
chess_move_list_t cpp_to_c_move_list(const std::set<simplechess::PieceMove>& moves);
chess_position_t cpp_to_c_position(const simplechess::Game& game);
chess_game_t cpp_to_c_game(const simplechess::Game& game);

// C to C++ conversions
simplechess::Color c_to_cpp_color(chess_color_t color);
simplechess::PieceType c_to_cpp_piece_type(chess_piece_type_t type);
simplechess::Square c_to_cpp_square(chess_square_t square);
simplechess::Piece c_to_cpp_piece(chess_piece_t piece);
simplechess::PieceMove c_to_cpp_move(chess_move_t move);

// Game reconstruction from history
simplechess::Game c_to_cpp_game(chess_game_manager_t manager, const chess_game_t& c_game);

// Utility functions
bool chess_position_to_fen(const chess_position_t& position, char* fen_buffer, size_t buffer_size);

// Additional conversion functions for FEN refactoring
simplechess::Board c_to_cpp_board(const chess_position_t& position);
uint8_t c_to_cpp_castling_rights(const chess_position_t& position);
std::optional<simplechess::Square> c_to_cpp_en_passant_target(const chess_position_t& position);

// Memory management helpers
void free_c_game_memory(chess_game_t* game);
chess_stage_t* allocate_stage_history(int count);
void free_stage_history(chess_stage_t* history, int count);

} // namespace conversion_utils

#endif