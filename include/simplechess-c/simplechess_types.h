#ifndef SIMPLE_CHESS_TYPES_H_30BE77B0_9C5F_11F0_9652_6FBB5A814FD9
#define SIMPLE_CHESS_TYPES_H_30BE77B0_9C5F_11F0_9652_6FBB5A814FD9

#include <stdint.h>
#include <stdbool.h>

/**
 * \file simplechess_types.h
 * \brief Type definitions for the Simple Chess Games C interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief The color of each side in a chess game.
	 */
	typedef enum simple_chess_color {
		SIMPLE_CHESS_COLOR_WHITE,
		SIMPLE_CHESS_COLOR_BLACK
	} simple_chess_color_t;

	/**
	 * \brief Represents a square on the chess board.
	 *
	 * Squares are identified by rank (1-8) and file ('a'-'h').
	 */
	typedef struct simple_chess_square {
		/** Rank (1-8) */
		uint8_t rank;

		/** File ('a'-'h') */
		char file;
	} simple_chess_square_t;

	/**
	 * \brief The type of a chess piece.
	 */
	typedef enum simple_chess_piece_type {
		SIMPLE_CHESS_PIECE_TYPE_PAWN,
		SIMPLE_CHESS_PIECE_TYPE_ROOK,
		SIMPLE_CHESS_PIECE_TYPE_KNIGHT,
		SIMPLE_CHESS_PIECE_TYPE_BISHOP,
		SIMPLE_CHESS_PIECE_TYPE_QUEEN,
		SIMPLE_CHESS_PIECE_TYPE_KING
	} simple_chess_piece_type_t;

	/**
	 * \brief Represents a chess piece with its type and color.
	 */
	typedef struct simple_chess_piece {
		/**
		 * \brief Type of the piece
		 */
		simple_chess_piece_type_t type;

		/**
		 * \brief Color of the piece
		 */
		simple_chess_color_t color;
	} simple_chess_piece_t;

	/**
	 * \brief Describes a move which can be made by a player.
	 *
	 * The move is simply described by the piece which moves, two squares (the
	 * square in which the piece was originally located and the square in which
	 * ended up) and, in the case of pawn promotions, the piece to which it
	 * promoted.
	 *
	 * This description is independent of any board state, and it only
	 * represents a move of a piece in the abstract (e.g. "The knight on g1
	 * moved to f3"), without accounting for captures, checks, draw offers,
	 * etc.
	 */
	typedef struct simple_chess_piece_move {
		/**
		 * \brief The piece whose movement is described.
		 */
		simple_chess_piece_t piece;

		/**
		 * \brief The original square of the moved piece.
		 *
		 * \note In the case of castling, it refers to the original square
		 * of the King.
		 */
		simple_chess_square_t src;

		/**
		 * \brief The final square of the moved piece.
		 *
		 * \note In the case of castling, it refers to the final square
		 * of the King.
		 */
		simple_chess_square_t dst;

		/**
		 * \brief Indicates if the move represents a pawn promotion.
		 */
		bool is_promotion;

		/**
		 * \brief Returns the new type of the promoted pawn (only valid if
		 * is_promotion is true).
		 */
		simple_chess_piece_type_t promoted_to;
	} simple_chess_piece_move_t;

	/**
	 * \brief The different types of check which can be caused by a move.
	 */
	typedef enum simple_chess_check_type {
		/**
		 * \brief No check whatsoever.
		 */
		SIMPLE_CHESS_CHECK_TYPE_NONE,

		/**
		 * \brief Regular check (the other side still has valid moves to break the
		 * check).
		 */
		SIMPLE_CHESS_CHECK_TYPE_CHECK,

		/**
		 * \brief Checkmate (the other side has no valid moves).
		 */
		SIMPLE_CHESS_CHECK_TYPE_CHECKMATE
	} simple_chess_check_type_t;

	/**
	 * \brief Describes a move that has been played in a game of chess.
	 *
	 * A played move is defined both by the "abstract" piece movement
	 * information (e.g. "The bishop on b5 moved to c6") as well as context
	 * of the situation when it was played (e.g. "The bishop captured a rook",
	 * "The move included a draw offer", "The move resulted in checkmate",
	 * etc.).
	 */
	typedef struct simple_chess_played_move {
		/**
		 * \brief The description of the move of the piece as a \ref
		 * simple_chess_piece_move_t.
		 */
		simple_chess_piece_move_t move;

		/**
		 * \brief Indicates if the move captured an opposing piece.
		 */
		bool is_capture;

		/** \brief
		 * The opposing piece that was captured (only if is_capture is true)
		 */
		simple_chess_piece_t captured_piece;

		/**
		 * \brief The type of check delivered by the move.
		 */
		simple_chess_check_type_t check_type;

		/**
		 * \brief Whether the player offers a draw with this move.
		 */
		bool offers_draw;

		/**
		 * \brief The string representation of the move in algebraic notation.
		 */
		char in_algebraic_notation[8];
	} simple_chess_played_move_t;

	/**
	 * \brief Castling rights bitfield values.
	 *
	 * These values can be combined using bitwise OR to represent
	 * multiple castling rights.
	 */
	typedef enum simple_chess_castling_right {
		/**
		 * \brief White can castle kingside
		 */
		SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE = 0x01,

		/**
		 * \brief White can castle queenside
		 */
		SIMPLE_CHESS_CASTLING_RIGHT_WHITE_QUEEN_SIDE = 0x02,

		/**
		 * \brief Black can castle kingside
		 */
		SIMPLE_CHESS_CASTLING_RIGHT_BLACK_KING_SIDE = 0x04,

		/**
		 * \brief Black can castle queenside
		 */
		SIMPLE_CHESS_CASTLING_RIGHT_BLACK_QUEEN_SIDE = 0x08
	} simple_chess_castling_right_t;

	/**
	 * \brief Represents the chess board state.
	 *
	 * The board is represented as a 64-element array where each element
	 * corresponds to a square. Squares are indexed from "a1"=0 to "h8"=63.
	 *
	 * Auxiliary functions are provided to convert from simple_chess_square_t to index and
	 * viceversa.
	 */
	typedef struct simple_chess_board {
		/**
		 * \brief Whether the i-th square is occupied by a piece or not.
		 */
		bool occupied[64];

		/**
		 * \brief The piece located at the i-th square (only if occupied[i] is true).
		 */
		simple_chess_piece_t piece_at[64];
	} simple_chess_board_t;

	/**
	 * \brief Represents a complete game position.
	 *
	 * This structure contains all information needed to describe a chess
	 * position, including board state, game rules state, and position
	 * metadata.
	 */
	typedef struct simple_chess_game_stage {
		/**
		 * \brief Current board position
		 */
		simple_chess_board_t board;

		/**
		 * \brief Whose turn it is
		 */
		simple_chess_color_t active_color;

		/**
		 * \brief Bitfield of available castling rights
		 */
		uint8_t castling_rights;

		/**
		 * \brief Counter for the 50-move rule
		 */
		uint16_t half_moves_since_last_capture_or_pawn_advance;

		/**
		 * \brief Full move number (incremented after Black's move)
		 */
		uint16_t full_moves;

		/**
		 * \brief Whether en passant capture is possible
		 */
		bool has_en_passant_target;

		/**
		 * \brief En passant target square (if has_en_passant_target is true)
		 */
		simple_chess_square_t en_passant_target;

		/**
		 * \brief Current check status
		 */
		simple_chess_check_type_t check_status;

		/**
		 * \brief FEN representation of this position
		 */
		char fen[90];
	} simple_chess_game_stage_t;

	/**
	 * \brief The overall state of a chess game.
	 */
	typedef enum simple_chess_game_state {
		/**
		 * \brief The game is still being played
		 */
		SIMPLE_CHESS_GAME_STATE_PLAYING,

		/**
		 * \brief The game ended in a draw
		 */
		SIMPLE_CHESS_GAME_STATE_DRAWN,

		/**
		 * \brief The game ended with a victory for white
		 */
		SIMPLE_CHESS_GAME_STATE_WHITE_WON,

		/**
		 * \brief The game ended with a victory for black
		 */
		SIMPLE_CHESS_GAME_STATE_BLACK_WON
	} simple_chess_game_state_t;

	/**
	 * \brief Reasons why a game might be drawn.
	 */
	typedef enum simple_chess_draw_reason {
		/**
		 * \brief Stalemate (the active side has no valid moves and is not in
		 * check).
		 */
		SIMPLE_CHESS_DRAW_REASON_STALEMATE,

		/**
		 * \brief Neither side has sufficient material to mate the other.
		 */
		SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL,

		/**
		 * \brief A side offered a draw and it was accepted.
		 */
		SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED,

		/**
		 * \brief The same position has been reached 3 times or will be
		 * reached for the thrid time with the next move.
		 *
		 * \note See FIDE rule 9.2.1 * and 9.2.2.
		 */
		SIMPLE_CHESS_DRAW_REASON_THREEFOLD_REPETITION,

		/**
		 * \brief The same position has been reached 5 times.
		 *
		 * \note See FIDE rule 9.6.1.
		 */
		SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION,

		/**
		 * \brief At least fifty full moves (i.e. each side has played their
		 * move) have been played (or will have been played after next move)
		 * since the last capture or pawn move.
		 *
		 * \note See FIDE rules 9.3.1 and 9.3.2.
		 */
		SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE,

		/**
		 * \brief The opponent (inactive side) has only a king remaining,
		 * making it impossible for them to win. The active side may claim
		 * a draw. This is never automatically enforced.
		 */
		SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL,

		/**
		 * \brief At least seventy-five full moves (i.e. each side has played their
		 * move) have been played since the last capture or pawn move.
		 *
		 * \note See FIDE rule 9.6.2. If the 75th move results in checkmate,
		 * the checkmate takes precedence over the draw rule.
		 */
		SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE
	} simple_chess_draw_reason_t;

	/**
	 * \brief Represents one entry in the game history.
	 *
	 * Each entry contains the position before a move was played
	 * and the move that was played.
	 */
	typedef struct simple_chess_game_history_entry {
		/**
		 * \brief FEN representation before this move
		 */
		char fen[90];

		/**
		 *
		 * \brief The move that was played
		 */
		simple_chess_played_move_t played_move;
	} simple_chess_game_history_entry_t;

	/**
	 * \brief Controls whether mandatory draw rules are automatically
	 * enforced or only claimable.
	 */
	typedef enum simple_chess_draw_enforcement {
		/**
		 * \brief Mandatory FIDE draw conditions are automatically enforced.
		 */
		SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC,

		/**
		 * \brief Mandatory FIDE draw conditions are only claimable, not
		 * automatically enforced.
		 */
		SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY
	} simple_chess_draw_enforcement_t;

	/**
	 * \brief Represents a complete chess game with all state information.
	 *
	 * This is the main structure for interacting with chess games.
	 * It contains the current position, game history, available moves,
	 * and game status information.
	 */
	typedef struct simple_chess_game {
		/**
		 * \brief Current game state
		 */
		simple_chess_game_state_t state;

		/**
		 * \brief Reason for draw (if state is SIMPLE_CHESS_GAME_STATE_DRAWN)
		 */
		simple_chess_draw_reason_t draw_reason;

		/**
		 * \brief Array of all moves played in the game
		 */
		simple_chess_game_history_entry_t* history;

		/**
		 * \brief Number of moves in history
		 */
		uint16_t history_size;

		/**
		 * \brief Array of all legal moves in current position
		 */
		simple_chess_piece_move_t* available_moves;

		/**
		 * \brief Number of available moves
		 */
		uint16_t available_move_count;

		/**
		 * \brief Current position and game state
		 */
		simple_chess_game_stage_t current_stage;

		/**
		 * \brief Whether a draw can be claimed by the current player
		 */
		bool is_draw_claimable;

		/**
		 *
		 * \brief Reason a draw can be claimed (if is_draw_claimable is true)
		 */
		simple_chess_draw_reason_t reason_to_claim_draw;

		/**
		 * \brief The draw enforcement mode of this game.
		 */
		simple_chess_draw_enforcement_t draw_enforcement;
	} simple_chess_game_t;

#ifdef __cplusplus
}
#endif

#endif
