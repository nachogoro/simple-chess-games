#include "conversion_utils.h"
#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/GameStage.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace conversion_utils {

// C++ to C conversions
chess_color_t cpp_to_c_color(simplechess::Color color) {
    return color == simplechess::Color::White ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK;
}

chess_piece_type_t cpp_to_c_piece_type(simplechess::PieceType type) {
    switch (type) {
        case simplechess::PieceType::Pawn:   return CHESS_PIECE_PAWN;
        case simplechess::PieceType::Rook:   return CHESS_PIECE_ROOK;
        case simplechess::PieceType::Knight: return CHESS_PIECE_KNIGHT;
        case simplechess::PieceType::Bishop: return CHESS_PIECE_BISHOP;
        case simplechess::PieceType::Queen:  return CHESS_PIECE_QUEEN;
        case simplechess::PieceType::King:   return CHESS_PIECE_KING;
    }
    return CHESS_PIECE_PAWN; // Default fallback
}

chess_game_state_t cpp_to_c_game_state(simplechess::GameState state) {
    switch (state) {
        case simplechess::GameState::Playing:  return CHESS_GAME_PLAYING;
        case simplechess::GameState::Drawn:    return CHESS_GAME_DRAWN;
        case simplechess::GameState::WhiteWon: return CHESS_GAME_WHITE_WON;
        case simplechess::GameState::BlackWon: return CHESS_GAME_BLACK_WON;
    }
    return CHESS_GAME_PLAYING; // Default fallback
}

chess_draw_reason_t cpp_to_c_draw_reason(simplechess::DrawReason reason) {
    switch (reason) {
        case simplechess::DrawReason::StaleMate:           return CHESS_DRAW_STALEMATE;
        case simplechess::DrawReason::InsufficientMaterial: return CHESS_DRAW_INSUFFICIENT_MATERIAL;
        case simplechess::DrawReason::OfferedAndAccepted:  return CHESS_DRAW_OFFERED_ACCEPTED;
        case simplechess::DrawReason::ThreeFoldRepetition: return CHESS_DRAW_THREEFOLD_REPETITION;
        case simplechess::DrawReason::FiveFoldRepetition:  return CHESS_DRAW_FIVEFOLD_REPETITION;
        case simplechess::DrawReason::FiftyMoveRule:       return CHESS_DRAW_FIFTY_MOVE;
        case simplechess::DrawReason::SeventyFiveMoveRule: return CHESS_DRAW_SEVENTY_FIVE_MOVE;
    }
    return CHESS_DRAW_STALEMATE; // Default fallback
}

chess_check_type_t cpp_to_c_check_type(simplechess::CheckType check_type) {
    switch (check_type) {
        case simplechess::CheckType::NoCheck:  return CHESS_CHECK_NONE;
        case simplechess::CheckType::Check:    return CHESS_CHECK_CHECK;
        case simplechess::CheckType::CheckMate: return CHESS_CHECK_CHECKMATE;
    }
    return CHESS_CHECK_NONE; // Default fallback
}

chess_square_t cpp_to_c_square(const simplechess::Square& square) {
    chess_square_t result;
    result.rank = square.rank();
    result.file = square.file();
    return result;
}

chess_piece_t cpp_to_c_piece(const simplechess::Piece& piece) {
    chess_piece_t result;
    result.type = cpp_to_c_piece_type(piece.type());
    result.color = cpp_to_c_color(piece.color());
    return result;
}

chess_move_t cpp_to_c_move(const simplechess::PieceMove& move) {
    chess_move_t result;
    result.piece = cpp_to_c_piece(move.piece());
    result.from = cpp_to_c_square(move.src());
    result.to = cpp_to_c_square(move.dst());
    result.is_promotion = move.promoted().has_value();
    if (result.is_promotion) {
        result.promotion = cpp_to_c_piece_type(move.promoted().value());
    } else {
        result.promotion = CHESS_PIECE_PAWN; // Default value
    }
    return result;
}

chess_move_list_t cpp_to_c_move_list(const std::set<simplechess::PieceMove>& moves) {
    chess_move_list_t result;
    result.count = std::min(static_cast<int>(moves.size()), CHESS_MAX_MOVES);

    int i = 0;
    for (const auto& move : moves) {
        if (i >= CHESS_MAX_MOVES) break;
        result.moves[i] = cpp_to_c_move(move);
        i++;
    }

    return result;
}

chess_position_t cpp_to_c_position(const simplechess::Game& game) {
    chess_position_t result = {};

    // Clear the position first
    for (int i = 0; i < CHESS_BOARD_SIZE; i++) {
        result.occupied[i] = false;
    }

    // Get the current board from game's current stage
    const auto& currentStage = game.currentStage();
    const auto& board = currentStage.board();

    // Fill in piece positions
    const auto& occupiedSquares = board.occupiedSquares();
    for (const auto& [square, piece] : occupiedSquares) {
        int index = (square.rank() - 1) * 8 + (square.file() - 'a');
        if (index >= 0 && index < CHESS_BOARD_SIZE) {
            result.pieces[index] = cpp_to_c_piece(piece);
            result.occupied[index] = true;
        }
    }

    result.active_color = cpp_to_c_color(game.activeColor());

    // Castling rights - extract from bitmask
    uint8_t castlingRights = currentStage.castlingRights();
    result.can_castle_kingside[CHESS_COLOR_WHITE] = (castlingRights & simplechess::CastlingRight::WhiteKingSide) != 0;
    result.can_castle_kingside[CHESS_COLOR_BLACK] = (castlingRights & simplechess::CastlingRight::BlackKingSide) != 0;
    result.can_castle_queenside[CHESS_COLOR_WHITE] = (castlingRights & simplechess::CastlingRight::WhiteQueenSide) != 0;
    result.can_castle_queenside[CHESS_COLOR_BLACK] = (castlingRights & simplechess::CastlingRight::BlackQueenSide) != 0;

    // En passant - need to parse from FEN or determine from last move
    // For now, set as not available (this would need more sophisticated parsing)
    result.has_en_passant = false;
    result.en_passant_target = {0, 'a'}; // Default value

    result.halfmove_clock = currentStage.halfMovesSinceLastCaptureOrPawnAdvance();
    result.fullmove_number = currentStage.fullMoveCounter();

    // Determine check status from game state
    simplechess::GameState gameState = game.gameState();

    if (gameState == simplechess::GameState::Playing) {
        // Use the check status from the current stage
        result.check_status = static_cast<chess_check_type_t>(currentStage.checkStatus());
    } else if (gameState == simplechess::GameState::WhiteWon || gameState == simplechess::GameState::BlackWon) {
        // Game ended with checkmate
        result.check_status = CHESS_CHECK_CHECKMATE;
    } else {
        // Game drawn or other state
        result.check_status = CHESS_CHECK_NONE;
    }

    return result;
}

chess_game_t cpp_to_c_game(const simplechess::Game& game) {
    chess_game_t result = {};

    result.state = cpp_to_c_game_state(game.gameState());

    if (game.gameState() == simplechess::GameState::Drawn) {
        result.draw_reason = cpp_to_c_draw_reason(game.drawReason());
        result.has_draw_reason = true;
    } else {
        result.has_draw_reason = false;
    }

    result.position = cpp_to_c_position(game);

    const auto& claimableDrawReason = game.reasonToClaimDraw();
    if (claimableDrawReason.has_value()) {
        result.claimable_draw = cpp_to_c_draw_reason(claimableDrawReason.value());
        result.can_claim_draw = true;
    } else {
        result.can_claim_draw = false;
    }

    // Extract game history as chess_stage_t array
    const auto& history = game.history();
    result.history_count = static_cast<int>(history.size());

    if (result.history_count > 0) {
        // Allocate stage array
        result.history = allocate_stage_history(result.history_count);

        for (int i = 0; i < result.history_count; i++) {
            const auto& stage = history[i];

            // Create a temporary game from this stage's FEN to get the exact position at that time
            try {
                simplechess::GameManager temp_manager;
                simplechess::Game temp_game = temp_manager.createGameFromFen(stage.first.fen());
                result.history[i].position = cpp_to_c_position(temp_game);
            } catch (...) {
                // Fallback to using current game position if FEN parsing fails
                result.history[i].position = cpp_to_c_position(game);
            }

            // Store move information (first position has no move)
            if (i == 0) {
                // Initial position - no move
                result.history[i].has_move = false;
                result.history[i].move = {}; // Zero-initialized
                result.history[i].draw_offer = false;
            } else {
                // Convert the move that led to this position
                const auto& played_move = stage.second;
                result.history[i].has_move = true;
                result.history[i].move = cpp_to_c_move(played_move.pieceMove());
                result.history[i].draw_offer = played_move.isDrawOffered();
            }
        }
    } else {
        result.history = nullptr;
    }

    return result;
}

// C to C++ conversions
simplechess::Color c_to_cpp_color(chess_color_t color) {
    return color == CHESS_COLOR_WHITE ? simplechess::Color::White : simplechess::Color::Black;
}

simplechess::PieceType c_to_cpp_piece_type(chess_piece_type_t type) {
    switch (type) {
        case CHESS_PIECE_PAWN:   return simplechess::PieceType::Pawn;
        case CHESS_PIECE_ROOK:   return simplechess::PieceType::Rook;
        case CHESS_PIECE_KNIGHT: return simplechess::PieceType::Knight;
        case CHESS_PIECE_BISHOP: return simplechess::PieceType::Bishop;
        case CHESS_PIECE_QUEEN:  return simplechess::PieceType::Queen;
        case CHESS_PIECE_KING:   return simplechess::PieceType::King;
    }
    return simplechess::PieceType::Pawn; // Default fallback
}

simplechess::Square c_to_cpp_square(chess_square_t square) {
    return simplechess::Square::fromRankAndFile(square.rank, square.file);
}

simplechess::Piece c_to_cpp_piece(chess_piece_t piece) {
    return simplechess::Piece(c_to_cpp_piece_type(piece.type), c_to_cpp_color(piece.color));
}

simplechess::PieceMove c_to_cpp_move(chess_move_t move) {
    simplechess::Piece piece = c_to_cpp_piece(move.piece);
    simplechess::Square from = c_to_cpp_square(move.from);
    simplechess::Square to = c_to_cpp_square(move.to);

    if (move.is_promotion) {
        return simplechess::PieceMove::pawnPromotion(piece, from, to, c_to_cpp_piece_type(move.promotion));
    } else {
        return simplechess::PieceMove::regularMove(piece, from, to);
    }
}

// Game reconstruction from history - simplified approach
simplechess::Game c_to_cpp_game(chess_game_manager_t manager, const chess_game_t& c_game) {
    if (!manager) {
        throw std::invalid_argument("Invalid game manager");
    }

    // For now, create a new game as a fallback
    // TODO: Implement proper reconstruction from chess_stage_t history
    return manager->manager->createNewGame();
}

// Memory management helpers
void free_c_game_memory(chess_game_t* game) {
    if (!game) return;

    if (game->history) {
        free_stage_history(game->history, game->history_count);
        game->history = nullptr;
    }

    game->history_count = 0;
}

chess_stage_t* allocate_stage_history(int count) {
    if (count <= 0) return nullptr;
    return static_cast<chess_stage_t*>(malloc(count * sizeof(chess_stage_t)));
}

void free_stage_history(chess_stage_t* history, int count) {
    if (!history) return;

    // chess_stage_t contains plain data structures, so just free the array
    free(history);
}

} // namespace conversion_utils
