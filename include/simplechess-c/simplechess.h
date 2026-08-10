#ifndef SIMPLE_CHESS_C_H_58ADD4DC_9C5F_11F0_B0FF_1BA779F2B9FB
#define SIMPLE_CHESS_C_H_58ADD4DC_9C5F_11F0_B0FF_1BA779F2B9FB

/**
 * \file simplechess.h
 * \brief Main C interface for the Simple Chess Games library.
 *
 * This header provides a C API for chess game logic, move validation,
 * and game state management. All functions follow C conventions and
 * return NULL for error conditions instead of throwing exceptions.
 */

#include "simplechess_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A human-readable description of \p error.
 *
 * Provided so that a caller passing errors on - a language binding
 * building an exception message, say - does not have to carry its own copy
 * of the enumeration.
 *
 * \param error The error to describe.
 *
 * \return A static string, never NULL, valid for the lifetime of the
 *         program.
 */
const char* simple_chess_error_string(simple_chess_error_t error);

/**
 * \brief Factory function to create a new game from the standard starting position.
 *
 * Creates a new chess game in the initial starting position with all pieces
 * in their standard locations.
 *
 * \param draw_enforcement Controls whether mandatory FIDE draw conditions
 *        are automatically enforced or only claimable.
 *
 * \return Pointer to new game object, or NULL on memory allocation failure.
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_create_new_game(
		simple_chess_draw_enforcement_t draw_enforcement,
		simple_chess_error_t* error);

/**
 * \brief Factory function to create a new game from a given board position.
 *
 * The original position of the board is given as a string in
 * Forsyth-Edwards Notation (FEN).
 *
 * \note FEN descriptions only give limited information about the
 *       history of the game. In particular, one cannot enforce certain
 *       drawing rules (threefold repetition). Hence, the history of the
 *       resulting game will not necessarily be complete.
 *
 * \param fen The representation of the initial position in
 *            Forsyth-Edwards Notation. Must be a valid FEN string.
 * \param draw_enforcement Controls whether mandatory FIDE draw conditions
 *        are automatically enforced or only claimable.
 *
 * \return Pointer to new game object, or NULL if the FEN string is invalid
 *         or memory allocation fails.
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_create_game_from_fen(
		const char* fen,
		simple_chess_draw_enforcement_t draw_enforcement,
		simple_chess_error_t* error);

/**
 * \brief Make a move for the player whose turn it is to play.
 *
 * Creates a new game state with the specified move applied. The original
 * game object remains unchanged.
 *
 * \param game Current game state. Must not be NULL.
 * \param move The move to make. Must be a legal move in the current position.
 *
 * \return Pointer to new game state after the move, or NULL in the following
 *         circumstances:
 *         - The game has already concluded (state is not SIMPLE_CHESS_GAME_STATE_PLAYING)
 *         - The move is not valid for the current player
 *         - Memory allocation failure
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_make_move(
		const simple_chess_game_t* game,
		simple_chess_piece_move_t move,
		simple_chess_error_t* error);

/**
 * \brief Make a move and optionally offer a draw.
 *
 * Creates a new game state with the specified move applied and optionally
 * offers a draw. The original game object remains unchanged.
 *
 * \param game Current game state. Must not be NULL.
 * \param move The move to make. Must be a legal move in the current position.
 * \param offer_draw True to offer a draw with this move, false otherwise.
 *
 * \return Pointer to new game state after the move, or NULL in the following
 *         circumstances:
 *         - The game has already concluded (state is not SIMPLE_CHESS_GAME_STATE_PLAYING)
 *         - The move is not valid for the current player
 *         - Memory allocation failure
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_make_move_with_draw_offer(
		const simple_chess_game_t* game,
		simple_chess_piece_move_t move,
		bool offer_draw,
		simple_chess_error_t* error);

/**
 * \brief Claim a draw if one is available.
 *
 * Claims a draw based on chess rules such as the fifty-move rule,
 * threefold repetition, or insufficient material. A draw can only
 * be claimed if the game's is_draw_claimable field is true.
 *
 * \param game Current game state. Must not be NULL.
 *
 * \return Pointer to new game state with draw claimed, or NULL if:
 *         - No draw is claimable in the current position
 *         - The game has already concluded
 *         - Memory allocation failure
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_claim_draw(
		const simple_chess_game_t* game,
		simple_chess_error_t* error);

/**
 * \brief Resign the game for the specified player.
 *
 * Creates a new game state where the specified player has resigned,
 * ending the game.
 *
 * \param game Current game state. Must not be NULL.
 * \param resigner Color of the player who is resigning.
 *
 * \return Pointer to new game state with resignation recorded, or NULL if:
 *         - The game has already concluded
 *         - Memory allocation failure
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_resign(
		const simple_chess_game_t* game,
		simple_chess_color_t resigner,
		simple_chess_error_t* error);

/**
 * \brief The overall state of the game.
 *
 * \param game Game to query. Must not be NULL.
 *
 * \return Whether the game is still being played, drawn, or won.
 */
simple_chess_game_state_t simple_chess_game_state(const simple_chess_game_t* game);

/**
 * \brief The color whose turn it is to play.
 *
 * \param game Game to query. Must not be NULL.
 *
 * \return The color to move next.
 */
simple_chess_color_t simple_chess_game_active_color(const simple_chess_game_t* game);

/**
 * \brief The draw enforcement mode the game was created with.
 *
 * \param game Game to query. Must not be NULL.
 *
 * \return The draw enforcement mode.
 */
simple_chess_draw_enforcement_t simple_chess_game_draw_enforcement(
		const simple_chess_game_t* game);

/**
 * \brief Why the game ended in a draw.
 *
 * \param game Game to query.
 * \param out Filled with the reason. Untouched if the game is not drawn.
 *
 * \return true if the game ended in a draw, false otherwise.
 */
bool simple_chess_game_draw_reason(
		const simple_chess_game_t* game,
		simple_chess_draw_reason_t* out);

/**
 * \brief Why the current player may claim a draw.
 *
 * \param game Game to query.
 * \param out Filled with the reason. Untouched if no draw can be claimed.
 *
 * \return true if the current player may claim a draw, false otherwise.
 */
bool simple_chess_game_reason_to_claim_draw(
		const simple_chess_game_t* game,
		simple_chess_draw_reason_t* out);

/**
 * \brief The position the game currently stands in.
 *
 * \param game Game to query.
 * \param out Filled with the current position.
 *
 * \return true on success, false if either argument is NULL.
 */
bool simple_chess_game_current_stage(
		const simple_chess_game_t* game,
		simple_chess_game_stage_t* out);

/**
 * \brief How many moves have been played.
 *
 * \param game Game to query. Must not be NULL.
 *
 * \return The number of entries in the history.
 */
uint16_t simple_chess_game_history_size(const simple_chess_game_t* game);

/**
 * \brief The position the \p index-th move of the game was played from.
 *
 * Unlike the FEN string the history used to carry, this is a full
 * position, so a past state of the game can be shown without parsing
 * anything.
 *
 * \param game Game to query.
 * \param index Which move, from 0 to simple_chess_game_history_size() - 1.
 * \param out Filled with the position.
 *
 * \return true on success, false if \p index is out of range or either
 *         pointer is NULL.
 */
bool simple_chess_game_history_stage(
		const simple_chess_game_t* game,
		uint16_t index,
		simple_chess_game_stage_t* out);

/**
 * \brief The \p index-th move played in the game.
 *
 * \param game Game to query.
 * \param index Which move, from 0 to simple_chess_game_history_size() - 1.
 * \param out Filled with the move.
 *
 * \return true on success, false if \p index is out of range or either
 *         pointer is NULL.
 */
bool simple_chess_game_history_move(
		const simple_chess_game_t* game,
		uint16_t index,
		simple_chess_played_move_t* out);

/**
 * \brief How many moves the current player may play.
 *
 * \param game Game to query. Must not be NULL.
 *
 * \return The number of legal moves in the current position.
 */
uint16_t simple_chess_game_available_move_count(const simple_chess_game_t* game);

/**
 * \brief The \p index-th legal move of the current player.
 *
 * The moves are in a stable order which does not depend on how they were
 * generated.
 *
 * \param game Game to query.
 * \param index Which move, from 0 to
 *        simple_chess_game_available_move_count() - 1.
 * \param out Filled with the move.
 *
 * \return true on success, false if \p index is out of range or either
 *         pointer is NULL.
 */
bool simple_chess_game_available_move(
		const simple_chess_game_t* game,
		uint16_t index,
		simple_chess_piece_move_t* out);

/**
 * \brief Duplicate a game.
 *
 * A game is opaque, so it cannot be copied by assigning one to another.
 *
 * \param game Game to copy. Must not be NULL.
 *
 * \return Pointer to a game equal to \p game, or NULL on failure.
 *
 * \param error If not NULL, set to SIMPLE_CHESS_OK on success or to the
 *        reason for the failure otherwise.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using simple_chess_destroy_game().
 */
simple_chess_game_t* simple_chess_copy_game(
		const simple_chess_game_t* game,
		simple_chess_error_t* error);

/**
 * \brief Unpacks what stands on a square into a piece.
 *
 * \param content What the board reports for the square.
 * \param out Filled with the piece standing there. Untouched if the square
 *            is empty.
 *
 * \return true if the square is occupied, false if it is empty.
 */
bool simple_chess_square_content_piece(
		simple_chess_square_content_t content,
		simple_chess_piece_t* out);

/**
 * \brief What a square holding \p piece reports.
 *
 * \param piece The piece standing on the square.
 *
 * \return The matching square content, never SIMPLE_CHESS_SQUARE_EMPTY.
 */
simple_chess_square_content_t simple_chess_square_content_from_piece(
		simple_chess_piece_t piece);

/**
 * \brief Convert a board index to a square.
 *
 * \param index Board index where 0=a1, 1=b1, ..., 63=h8.
 * \param out Filled with the square. Untouched if \p index is out of range.
 *
 * \return true on success, false if \p index is above 63 or \p out is NULL.
 */
bool simple_chess_square_from_index(uint8_t index, simple_chess_square_t* out);

/**
 * \brief Convert a square to a board index.
 *
 * \param square Square with a rank of 1-8 and a file of 'a'-'h'.
 * \param out Filled with the board index, where 0=a1, 1=b1, ..., 63=h8.
 *            Untouched if \p square is not a square of the board.
 *
 * \return true on success, false if \p square is off the board or \p out
 *         is NULL.
 */
bool simple_chess_index_from_square(simple_chess_square_t square, uint8_t* out);

/**
 * \brief Parse a square written in algebraic notation, such as "e4".
 *
 * The letter may be of either case.
 *
 * \param algebraic The square in algebraic notation.
 * \param out Filled with the square. Untouched if \p algebraic does not
 *            name one.
 *
 * \return true on success, false otherwise.
 */
bool simple_chess_square_from_string(
		const char* algebraic,
		simple_chess_square_t* out);

/**
 * \brief Write a square in algebraic notation, such as "e4".
 *
 * \param square The square to write.
 * \param out Buffer of at least three characters, filled with the two
 *            characters of the notation and a terminating NUL. Filled with
 *            an empty string if \p square is not a square of the board.
 */
void simple_chess_square_to_string(simple_chess_square_t square, char* out);

/**
 * \brief Whether \p square names a square of the board.
 *
 * \param square The square to check.
 *
 * \return true if the rank is 1-8 and the file 'a'-'h', false otherwise.
 */
bool simple_chess_square_is_valid(simple_chess_square_t square);

/**
 * \brief The colour a square of the board is painted.
 *
 * \param square The square whose colour is wanted. Must be a square of the
 *        board.
 *
 * \return The colour the square is painted.
 */
simple_chess_color_t simple_chess_square_color(simple_chess_square_t square);

/**
 * \brief Whether two moves describe the same thing.
 *
 * \param a First move.
 * \param b Second move.
 *
 * \return true if the moves are equal, false otherwise.
 */
bool simple_chess_piece_move_equals(
		simple_chess_piece_move_t a,
		simple_chess_piece_move_t b);

/**
 * \brief Find the legal move which goes from \p src to \p dst.
 *
 * A move names the piece which moves, which a caller who only knows the two
 * squares involved - a user interface reporting that a piece was dragged
 * from one square to another, say - would otherwise have to look up on the
 * board first. Searching the legal moves also means the result is known to
 * be playable.
 *
 * \note In the case of castling, \p src and \p dst refer to the original
 *       and final squares of the King.
 *
 * \param game The game whose legal moves are searched.
 * \param src The square the piece moves from.
 * \param dst The square the piece moves to.
 * \param is_promotion Whether the move sought promotes a pawn.
 * \param promoted_to The type the pawn is promoted to. Ignored unless
 *        \p is_promotion is true.
 * \param out Filled with the move. Untouched if there is no such move.
 *
 * \return true if the player to move has such a move, false otherwise.
 */
bool simple_chess_find_move(
		const simple_chess_game_t* game,
		simple_chess_square_t src,
		simple_chess_square_t dst,
		bool is_promotion,
		simple_chess_piece_type_t promoted_to,
		simple_chess_piece_move_t* out);

/**
 * \brief Free all memory associated with a game object.
 *
 * Properly deallocates all memory used by a game object, including
 * its history, available moves, and internal structures.
 *
 * \param game Game object to destroy. Can be NULL (no operation performed).
 *
 * \note After calling this function, the game pointer becomes invalid
 *       and must not be used.
 * \note This function must be called for every game object created by
 *       the library to avoid memory leaks.
 */
void simple_chess_destroy_game(simple_chess_game_t* game);

#ifdef __cplusplus
}
#endif

#endif
