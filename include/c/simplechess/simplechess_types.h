#ifndef SIMPLE_CHESS_TYPES_H_30BE77B0_9C5F_11F0_9652_6FBB5A814FD9
#define SIMPLE_CHESS_TYPES_H_30BE77B0_9C5F_11F0_9652_6FBB5A814FD9

#include <stdint.h>

enum color_t {
	ColorWhite,
	ColorBlack
};

struct square_t {
	uint8_t rank;
	char file;
	color_t color;
	char as_string[3];
};

enum piece_type_t {
	PieceTypePawn,
	PieceTypeRook,
	PieceTypeKnight,
	PieceTypeBishop,
	PieceTypeQueen,
	PieceTypeKing
};

struct piece_t {
	piece_type_t type;
	color_t color;
};

struct piece_move_t {
	piece_t piece;
	square_t src;
	square_t dst;
	bool is_promotion;
	piece_type_t promoted_to;
};

enum check_type_t {
	CheckTypeNone,
	CheckTypeCheck,
	CheckTypeCheckMate
};

struct played_move_t {
	piece_move_t move;
	bool is_capture;
	piece_t captured_piece;
	check_type_t check_type;
	bool offers_draw;
	char in_algebraic_notation[8];
};

enum castling_right_t {
	CastlingRightWhiteKingSide = 0x01,
	CastlingRightWhiteQueenSide = 0x02,
	CastlingRightBlackKingSide = 0x04,
	CastlingRightBlackQueenSide = 0x08
};

struct board_t {
	bool occupied[64];
	piece_t piece_at[64];
};

struct game_stage_t {
	board_t board;
	color_t active_color;
	uint8_t castling_rights;
	uint16_t half_moves_since_last_capture_or_pawn_advance;
	uint16_t full_moves;
	bool has_en_passant_target;
	square_t en_passant_target;
	check_type_t check_status;
	char fen[90];
};

enum game_state_t {
	GameStatePlaying,
	GameStateDrawn,
	GameStateWhiteWon,
	GameStateBlackWon
};

enum draw_reason_t {
	DrawReasonStaleMate,
	DrawReasonInsufficientMaterial,
	DrawReasonOfferedAndAccepted,
	DrawReasonThreeFoldRepetition,
	DrawReasonFiveFoldRepetition,
	DrawReasonFiftyMoveRule,
	DrawReasonSeventyFiveMoveRule
};

struct game_history_entry_t {
	char fen[90];
	played_move_t played_move;
};

struct game_t {
	game_state_t state;
	draw_reason_t draw_reason;
	game_history_entry_t* history;
	uint16_t history_size;
	piece_move_t* available_moves;
	uint16_t available_move_count;
	game_stage_t current_stage;
	bool is_draw_claimable;
	draw_reason_t reason_to_claim_draw;
};

#endif
