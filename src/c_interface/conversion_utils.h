#ifndef CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19
#define CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19

#include <simplechess-c/simplechess.h>
#include <simplechess/SimpleChess.h>


namespace conversion_utils {
	// C++ to C conversions
	simple_chess_color_t          c_color(simplechess::Color color);
	simple_chess_square_t         c_square(const simplechess::Square& square);
	simple_chess_piece_type_t     c_piece_type(simplechess::PieceType type);
	simple_chess_piece_t          c_piece(const simplechess::Piece& piece);
	simple_chess_piece_move_t     c_piece_move(const simplechess::PieceMove& move);
	simple_chess_check_type_t     c_check_type(simplechess::CheckType check);
	simple_chess_played_move_t    c_played_move(const simplechess::PlayedMove& move);
	simple_chess_castling_right_t c_castling_rights(simplechess::CastlingRight right);
	uint8_t          c_castling_rights(uint8_t rights);
	simple_chess_board_t          c_board(const simplechess::Board& board);
	simple_chess_game_stage_t     c_game_stage(const simplechess::GameStage& stage);
	simple_chess_game_state_t     c_game_state(simplechess::GameState state);
	simple_chess_draw_reason_t    c_draw_reason(simplechess::DrawReason reason);

	// C to C++ conversions
	simplechess::Color         cpp_color(simple_chess_color_t color);
	simplechess::Square        cpp_square(const simple_chess_square_t& square);
	simplechess::PieceType     cpp_piece_type(simple_chess_piece_type_t type);
	simplechess::Piece         cpp_piece(const simple_chess_piece_t& piece);
	simplechess::PieceMove     cpp_piece_move(const simple_chess_piece_move_t& move);
	simplechess::CheckType     cpp_check_type(simple_chess_check_type_t check);
	simplechess::PlayedMove    cpp_played_move(const simple_chess_played_move_t& move);
	simplechess::CastlingRight cpp_castling_right(simple_chess_castling_right_t right);
	uint8_t                    cpp_castling_rights(uint8_t rights);
	simplechess::Board         cpp_board(const simple_chess_board_t& board);
	simplechess::GameStage     cpp_game_stage(const simple_chess_game_stage_t& stage);
	simplechess::GameState     cpp_game_state(simple_chess_game_state_t state);
	simplechess::DrawReason    cpp_draw_reason(simple_chess_draw_reason_t reason);
	simplechess::DrawEnforcement cpp_draw_enforcement(simple_chess_draw_enforcement_t enforcement);
	simple_chess_draw_enforcement_t         c_draw_enforcement(simplechess::DrawEnforcement enforcement);
} // namespace conversion_utils

#endif
