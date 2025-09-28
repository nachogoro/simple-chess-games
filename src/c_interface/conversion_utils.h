#ifndef CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19
#define CONVERSION_UTILS_H_8B2A9F4D_7C1E_4B5A_9D6F_3E8C2A5B7D19

#include <c/simplechess/simplechess.h>
#include <cpp/simplechess/SimpleChess.h>


namespace conversion_utils {
	// C++ to C conversions
	color_t          c_color(simplechess::Color color);
	square_t         c_square(const simplechess::Square& square);
	piece_type_t     c_piece_type(simplechess::PieceType type);
	piece_t          c_piece(const simplechess::Piece& piece);
	piece_move_t     c_piece_move(const simplechess::PieceMove& move);
	check_type_t     c_check_type(simplechess::CheckType check);
	played_move_t    c_played_move(const simplechess::PlayedMove& move);
	castling_right_t c_castling_rights(simplechess::CastlingRight right);
	uint8_t          c_castling_rights(uint8_t rights);
	board_t          c_board(const simplechess::Board& board);
	game_stage_t     c_game_stage(const simplechess::GameStage& stage);
	game_state_t     c_game_state(simplechess::GameState state);
	draw_reason_t    c_draw_reason(simplechess::DrawReason reason);
	game_t*          c_game(const simplechess::Game& game);

	// C to C++ conversions
	simplechess::Color         cpp_color(color_t color);
	simplechess::Square        cpp_square(const square_t& square);
	simplechess::PieceType     cpp_piece_type(piece_type_t type);
	simplechess::Piece         cpp_piece(const piece_t& piece);
	simplechess::PieceMove     cpp_piece_move(const piece_move_t& move);
	simplechess::CheckType     cpp_check_type(check_type_t check);
	simplechess::PlayedMove    cpp_played_move(const played_move_t& move);
	simplechess::CastlingRight cpp_castling_right(castling_right_t right);
	uint8_t                    cpp_castling_rights(uint8_t rights);
	simplechess::Board         cpp_board(const board_t& board);
	simplechess::GameStage     cpp_game_stage(const game_stage_t& stage);
	simplechess::GameState     cpp_game_state(game_state_t state);
	simplechess::DrawReason    cpp_draw_reason(draw_reason_t reason);
	simplechess::Game          cpp_game(const game_t& game);
} // namespace conversion_utils

#endif
