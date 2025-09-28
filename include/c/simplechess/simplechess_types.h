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
	enum color_t {
		ColorWhite,
		ColorBlack
	};

	/**
	 * \brief Represents a square on the chess board.
	 *
	 * Squares are identified by rank (1-8) and file ('a'-'h').
	 */
	struct square_t {
		/** Rank (1-8) */
		uint8_t rank;

		/** File ('a'-'h') */
		char file;
	};

	/**
	 * \brief The type of a chess piece.
	 */
	enum piece_type_t {
		PieceTypePawn,
		PieceTypeRook,
		PieceTypeKnight,
		PieceTypeBishop,
		PieceTypeQueen,
		PieceTypeKing
	};

	/**
	 * \brief Represents a chess piece with its type and color.
	 */
	struct piece_t {
		/**
		 * \brief Type of the piece
		 */
		piece_type_t type;

		/**
		 * \brief Color of the piece
		 */
		color_t color;
	};

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
	struct piece_move_t {
		/**
		 * \brief The piece whose movement is described.
		 */
		piece_t piece;

		/**
		 * \brief The original square of the moved piece.
		 *
		 * \note In the case of castling, it refers to the original square
		 * of the King.
		 */
		square_t src;

		/**
		 * \brief The final square of the moved piece.
		 *
		 * \note In the case of castling, it refers to the final square
		 * of the King.
		 */
		square_t dst;

		/**
		 * \brief Indicates if the move represents a pawn promotion.
		 */
		bool is_promotion;

		/**
		 * \brief Returns the new type of the promoted pawn (only valid if
		 * is_promotion is true).
		 */
		piece_type_t promoted_to;
	};

	/**
	 * \brief The different types of check which can be caused by a move.
	 */
	enum check_type_t {
		/**
		 * \brief No check whatsoever.
		 */
		CheckTypeNone,

		/**
		 * \brief Regular check (the other side still has valid moves to break the
		 * check).
		 */
		CheckTypeCheck,

		/**
		 * \brief Checkmate (the other side has no valid moves).
		 */
		CheckTypeCheckMate
	};

	/**
	 * \brief Describes a move that has been played in a game of chess.
	 *
	 * A played move is defined both by the "abstract" piece movement
	 * information (e.g. "The bishop on b5 moved to c6") as well as context
	 * of the situation when it was played (e.g. "The bishop captured a rook",
	 * "The move included a draw offer", "The move resulted in checkmate",
	 * etc.).
	 */
	struct played_move_t {
		/**
		 * \brief The description of the move of the piece as a \ref
		 * piece_move_t.
		 */
		piece_move_t move;

		/**
		 * \brief Indicates if the move captured an opposing piece.
		 */
		bool is_capture;

		/** \brief
		 * The opposing piece that was captured (only if is_capture is true)
		 */
		piece_t captured_piece;

		/**
		 * \brief The type of check delivered by the move.
		 */
		check_type_t check_type;

		/**
		 * \brief Whether the player offers a draw with this move.
		 */
		bool offers_draw;

		/**
		 * \brief The string representation of the move in algebraic notation.
		 */
		char in_algebraic_notation[8];
	};

	/**
	 * \brief Castling rights bitfield values.
	 *
	 * These values can be combined using bitwise OR to represent
	 * multiple castling rights.
	 */
	enum castling_right_t {
		/**
		 * \brief White can castle kingside
		 */
		CastlingRightWhiteKingSide = 0x01,

		/**
		 * \brief White can castle queenside
		 */
		CastlingRightWhiteQueenSide = 0x02,

		/**
		 * \brief Black can castle kingside
		 */
		CastlingRightBlackKingSide = 0x04,

		/**
		 * \brief Black can castle queenside
		 */
		CastlingRightBlackQueenSide = 0x08
	};

	/**
	 * \brief Represents the chess board state.
	 *
	 * The board is represented as a 64-element array where each element
	 * corresponds to a square. Squares are indexed from "a1"=0 to "h8"=63.
	 *
	 * Auxiliary functions are provided to convert from square_t to index and
	 * viceversa.
	 */
	struct board_t {
		/**
		 * \brief Whether the i-th square is occupied by a piece or not.
		 */
		bool occupied[64];

		/**
		 * \brief The piece located at the i-th square (only if occupied[i] is true).
		 */
		piece_t piece_at[64];
	};

	/**
	 * \brief Represents a complete game position.
	 *
	 * This structure contains all information needed to describe a chess
	 * position, including board state, game rules state, and position
	 * metadata.
	 */
	struct game_stage_t {
		/**
		 * \brief Current board position
		 */
		board_t board;

		/**
		 * \brief Whose turn it is
		 */
		color_t active_color;

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
		square_t en_passant_target;

		/**
		 * \brief Current check status
		 */
		check_type_t check_status;

		/**
		 * \brief FEN representation of this position
		 */
		char fen[90];
	};

	/**
	 * \brief The overall state of a chess game.
	 */
	enum game_state_t {
		/**
		 * \brief The game is still being played
		 */
		GameStatePlaying,

		/**
		 * \brief The game ended in a draw
		 */
		GameStateDrawn,

		/**
		 * \brief The game ended with a victory for white
		 */
		GameStateWhiteWon,

		/**
		 * \brief The game ended with a victory for black
		 */
		GameStateBlackWon
	};

	/**
	 * \brief Reasons why a game might be drawn.
	 */
	enum draw_reason_t
	{
		/**
		 * \brief Stalemate (the active side has no valid moves and is not in
		 * check).
		 */
		DrawReasonStaleMate,

		/**
		 * \brief Neither side has sufficient material to mate the other.
		 */
		DrawReasonInsufficientMaterial,

		/**
		 * \brief A side offered a draw and it was accepted.
		 */
		DrawReasonOfferedAndAccepted,

		/**
		 * \brief The same position has been reached 3 times or will be
		 * reached for the thrid time with the next move.
		 *
		 * \note See FIDE rule 9.2.1 * and 9.2.2.
		 */
		DrawReasonThreeFoldRepetition,

		/**
		 * \brief The same position has been reached 5 times.
		 *
		 * \note See FIDE rule 9.6.1.
		 */
		DrawReasonFiveFoldRepetition,

		/**
		 * \brief At least fifty full moves (i.e. each side has played their
		 * move) have been played (or will have been played after next move)
		 * since the last capture or pawn move.
		 *
		 * \note See FIDE rules 9.3.1 and 9.3.2.
		 */
		DrawReasonFiftyMoveRule,

		/**
		 * \brief At least seventy-five full moves (i.e. each side has played their
		 * move) have been played since the last capture or pawn move.
		 *
		 * \note See FIDE rule 9.6.2. If the 75th move results in checkmate,
		 * the checkmate takes precedence over the draw rule.
		 */
		DrawReasonSeventyFiveMoveRule
	};

	/**
	 * \brief Represents one entry in the game history.
	 *
	 * Each entry contains the position before a move was played
	 * and the move that was played.
	 */
	struct game_history_entry_t {
		/**
		 * \brief FEN representation before this move
		 */
		char fen[90];

		/**
		 *
		 * \brief The move that was played
		 */
		played_move_t played_move;
	};

	/**
	 * \brief Represents a complete chess game with all state information.
	 *
	 * This is the main structure for interacting with chess games.
	 * It contains the current position, game history, available moves,
	 * and game status information.
	 */
	struct game_t {
		/**
		 * \brief Current game state
		 */
		game_state_t state;

		/**
		 * \brief Reason for draw (if state is GameStateDrawn)
		 */
		draw_reason_t draw_reason;

		/**
		 * \brief Array of all moves played in the game
		 */
		game_history_entry_t* history;

		/**
		 * \brief Number of moves in history
		 */
		uint16_t history_size;

		/**
		 * \brief Array of all legal moves in current position
		 */
		piece_move_t* available_moves;

		/**
		 * \brief Number of available moves
		 */
		uint16_t available_move_count;

		/**
		 * \brief Current position and game state
		 */
		game_stage_t current_stage;

		/**
		 * \brief Whether a draw can be claimed by the current player
		 */
		bool is_draw_claimable;

		/**
		 *
		 * \brief Reason a draw can be claimed (if is_draw_claimable is true)
		 */
		draw_reason_t reason_to_claim_draw;
	};

#ifdef __cplusplus
}
#endif

#endif
