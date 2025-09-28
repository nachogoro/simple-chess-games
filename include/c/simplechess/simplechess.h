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
 * \brief Factory function to create a new game from the standard starting position.
 *
 * Creates a new chess game in the initial starting position with all pieces
 * in their standard locations.
 *
 * \return Pointer to new game object, or NULL on memory allocation failure.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_create_new_game();

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
 *
 * \return Pointer to new game object, or NULL if the FEN string is invalid
 *         or memory allocation fails.
 *
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_create_game_from_fen(const char* fen);

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
 *         - The game has already concluded (state is not GameStatePlaying)
 *         - The move is not valid for the current player
 *         - Memory allocation failure
 *
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_make_move(const game_t* game, piece_move_t move);

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
 *         - The game has already concluded (state is not GameStatePlaying)
 *         - The move is not valid for the current player
 *         - Memory allocation failure
 *
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_make_move_with_draw_offer(const game_t* game, piece_move_t move, bool offer_draw);

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
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_claim_draw(const game_t* game);

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
 * \note The caller is responsible for freeing the returned game object
 *       using destroy_game().
 */
game_t* simple_chess_resign(const game_t* game, color_t resigner);

/**
 * \brief Convert a board index to a square structure.
 *
 * Converts a linear board index (0-63) to a square_t structure with
 * rank, file, color, and string representation.
 *
 * \param index Board index where 0=a1, 1=b1, ..., 63=h8.
 *              Must be in range 0-63.
 *
 * \return Square structure representing the position.
 *
 * \note If index is out of range, the behavior is undefined.
 */
square_t simple_chess_square_from_index(uint8_t index);

/**
 * \brief Convert a square structure to a board index.
 *
 * Converts a square_t structure to a linear board index (0-63).
 *
 * \param square Square structure with valid rank (1-8) and file ('a'-'h').
 *
 * \return Board index where 0=a1, 1=b1, ..., 63=h8.
 *
 * \note If the square has invalid rank or file values, the behavior
 *       is undefined.
 */
uint8_t simple_chess_index_from_square(square_t square);

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
void destroy_game(game_t* game);

#ifdef __cplusplus
}
#endif

#endif
