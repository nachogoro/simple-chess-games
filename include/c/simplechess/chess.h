#ifndef CHESS_C_H_F3A8B2E7_9D4C_4A1E_8F5B_2C7D9E6A3F18
#define CHESS_C_H_F3A8B2E7_9D4C_4A1E_8F5B_2C7D9E6A3F18

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// ===== ENUMS =====
typedef enum {
    CHESS_COLOR_WHITE = 0,
    CHESS_COLOR_BLACK = 1
} chess_color_t;

typedef enum {
    CHESS_PIECE_PAWN = 0,
    CHESS_PIECE_ROOK = 1,
    CHESS_PIECE_KNIGHT = 2,
    CHESS_PIECE_BISHOP = 3,
    CHESS_PIECE_QUEEN = 4,
    CHESS_PIECE_KING = 5
} chess_piece_type_t;

typedef enum {
    CHESS_GAME_PLAYING = 0,
    CHESS_GAME_DRAWN = 1,
    CHESS_GAME_WHITE_WON = 2,
    CHESS_GAME_BLACK_WON = 3
} chess_game_state_t;

typedef enum {
    CHESS_DRAW_STALEMATE = 0,
    CHESS_DRAW_INSUFFICIENT_MATERIAL = 1,
    CHESS_DRAW_OFFERED_ACCEPTED = 2,
    CHESS_DRAW_THREEFOLD_REPETITION = 3,
    CHESS_DRAW_FIVEFOLD_REPETITION = 4,
    CHESS_DRAW_FIFTY_MOVE = 5,
    CHESS_DRAW_SEVENTY_FIVE_MOVE = 6
} chess_draw_reason_t;

typedef enum {
    CHESS_CHECK_NONE = 0,
    CHESS_CHECK_CHECK = 1,
    CHESS_CHECK_CHECKMATE = 2
} chess_check_type_t;

// ===== STRUCTS =====
typedef struct {
    uint8_t rank;  // 1-8
    char file;     // 'a'-'h'
} chess_square_t;

typedef struct {
    chess_piece_type_t type;
    chess_color_t color;
} chess_piece_t;

typedef struct {
    chess_piece_t piece;
    chess_square_t from;
    chess_square_t to;
    chess_piece_type_t promotion;  // Only valid if is_promotion is true
    bool is_promotion;
} chess_move_t;

#define CHESS_MAX_MOVES 256
typedef struct {
    chess_move_t moves[CHESS_MAX_MOVES];
    int count;
} chess_move_list_t;

#define CHESS_BOARD_SIZE 64
typedef struct {
    chess_piece_t pieces[CHESS_BOARD_SIZE];  // Indexed by rank*8 + file
    bool occupied[CHESS_BOARD_SIZE];         // Which squares have pieces

    chess_color_t active_color;

    bool can_castle_kingside[2];   // [WHITE, BLACK]
    bool can_castle_queenside[2];  // [WHITE, BLACK]

    chess_square_t en_passant_target;  // {0,0} if none
    bool has_en_passant;

    int halfmove_clock;
    int fullmove_number;

    chess_check_type_t check_status;  // Check status for the active player
} chess_position_t;

// Historical position access
typedef struct {
    chess_position_t position;
    chess_move_t move;      // Move that led to this position (invalid for initial position)
    bool has_move;          // Whether this stage has an associated move
    bool draw_offer;        // Whether the move included a draw offer
} chess_stage_t;

typedef struct {
    chess_game_state_t state;
    chess_draw_reason_t draw_reason;  // Only valid if has_draw_reason is true
    bool has_draw_reason;
    chess_position_t position;
    chess_draw_reason_t claimable_draw;  // Reason current player can claim draw
    bool can_claim_draw;

    chess_stage_t* history;    // Array of stages for this game
    int history_count;         // Number of positions in history
} chess_game_t;

// ===== OPAQUE HANDLE =====
typedef struct chess_game_manager* chess_game_manager_t;

// ===== CORE API FUNCTIONS =====

// Game Manager (minimal opaque pointer usage)
chess_game_manager_t chess_create_manager(void);
void chess_destroy_manager(chess_game_manager_t manager);

// Game Creation
bool chess_create_new_game(chess_game_manager_t manager, chess_game_t* game_out);
bool chess_create_game_from_fen(chess_game_manager_t manager, const char* fen, chess_game_t* game_out);

// Game Operations
bool chess_make_move(chess_game_manager_t manager,
                     const chess_game_t* current_game,
                     const chess_move_t* move,
                     bool offer_draw,
                     chess_game_t* result_game);

bool chess_claim_draw(chess_game_manager_t manager,
                      const chess_game_t* current_game,
                      chess_game_t* result_game);

bool chess_resign(chess_game_manager_t manager,
                  const chess_game_t* current_game,
                  chess_color_t resigning_player,
                  chess_game_t* result_game);

// ===== MEMORY MANAGEMENT =====

// Destroy a game and free its memory (call after you're done with a game)
void chess_destroy_game(chess_game_t* game);

// ===== UTILITY FUNCTIONS (no opaque pointers) =====

// Square utilities
chess_square_t chess_square_from_string(const char* square_str);
void chess_square_to_string(chess_square_t square, char* out_str); // out_str must be 3+ chars
bool chess_square_is_valid(chess_square_t square);
int chess_square_to_index(chess_square_t square);  // Returns 0-63 for array indexing
chess_square_t chess_square_from_index(int index);

// Move utilities
bool chess_move_is_valid_format(const chess_move_t* move);
void chess_move_to_string(const chess_move_t* move, char* out_str); // out_str must be 6+ chars

// Position utilities
chess_move_list_t chess_get_moves_for_piece(const chess_position_t* position, chess_square_t square);
chess_move_list_t chess_get_all_moves_(const chess_position_t* position);
bool chess_get_fen_for_position(const chess_position_t* position, char* out_buffer);
chess_piece_t chess_get_piece_at(const chess_position_t* position, chess_square_t square);
bool chess_is_square_occupied(const chess_position_t* position, chess_square_t square);

#ifdef __cplusplus
}
#endif

#endif
