#include "conversion_utils.h"
#include "simplechess/SimpleChess.h"
#include "../core/Builders.h"
#include "../core/details/fen/FenUtils.h"
#include <algorithm>
#include <cstring>

// C++ to C conversions
simple_chess_color_t conversion_utils::c_color(simplechess::Color color) {
	switch (color) {
		case simplechess::Color::White:
			return SIMPLE_CHESS_COLOR_WHITE;
		case simplechess::Color::Black:
			return SIMPLE_CHESS_COLOR_BLACK;
	}

	// Supress warning
	return SIMPLE_CHESS_COLOR_WHITE;
}

simple_chess_square_t conversion_utils::c_square(const simplechess::Square& square) {
	return {square.rank(), square.file()};
}

simple_chess_piece_type_t conversion_utils::c_piece_type(simplechess::PieceType type) {
	switch (type) {
		case simplechess::PieceType::Pawn:
			return SIMPLE_CHESS_PIECE_TYPE_PAWN;
		case simplechess::PieceType::Rook:
			return SIMPLE_CHESS_PIECE_TYPE_ROOK;
		case simplechess::PieceType::Knight:
			return SIMPLE_CHESS_PIECE_TYPE_KNIGHT;
		case simplechess::PieceType::Bishop:
			return SIMPLE_CHESS_PIECE_TYPE_BISHOP;
		case simplechess::PieceType::Queen:
			return SIMPLE_CHESS_PIECE_TYPE_QUEEN;
		case simplechess::PieceType::King:
			return SIMPLE_CHESS_PIECE_TYPE_KING;
	}

	// Suppress warning
	return SIMPLE_CHESS_PIECE_TYPE_PAWN;
}

simple_chess_piece_t conversion_utils::c_piece(const simplechess::Piece& piece) {
	return {c_piece_type(piece.type()), c_color(piece.color())};
}

simple_chess_piece_move_t conversion_utils::c_piece_move(const simplechess::PieceMove& move) {
	return {
		c_piece(move.piece()),
		c_square(move.src()),
		c_square(move.dst()),
		move.promoted().has_value(),
		(move.promoted().has_value() ? c_piece_type(*move.promoted())
		 : SIMPLE_CHESS_PIECE_TYPE_PAWN)
	};
}

simple_chess_check_type_t conversion_utils::c_check_type(simplechess::CheckType check)
{
	switch (check) {
		case simplechess::CheckType::NoCheck:
			return SIMPLE_CHESS_CHECK_TYPE_NONE;
		case simplechess::CheckType::Check:
			return SIMPLE_CHESS_CHECK_TYPE_CHECK;
		case simplechess::CheckType::CheckMate:
			return SIMPLE_CHESS_CHECK_TYPE_CHECKMATE;
	}

	// Suppress warning
	return SIMPLE_CHESS_CHECK_TYPE_NONE;
};

simple_chess_played_move_t conversion_utils::c_played_move(const simplechess::PlayedMove& move) {
	simple_chess_played_move_t result;
	result.move = c_piece_move(move.pieceMove());

	result.is_capture = move.capturedPiece().has_value();
	if (result.is_capture) {
		result.captured_piece = c_piece(*move.capturedPiece());
	}

	result.check_type = c_check_type(move.checkType());
	result.offers_draw = move.isDrawOffered();
	strncpy(result.in_algebraic_notation, move.inAlgebraicNotation().c_str(), sizeof(result.in_algebraic_notation) - 1);
	result.in_algebraic_notation[sizeof(result.in_algebraic_notation) - 1] = '\0';
	return result;
}

simple_chess_castling_right_t conversion_utils::c_castling_rights(simplechess::CastlingRight right) {
	switch (right) {
		case simplechess::CastlingRight::WhiteKingSide:
			return SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE;
		case simplechess::CastlingRight::WhiteQueenSide:
			return SIMPLE_CHESS_CASTLING_RIGHT_WHITE_QUEEN_SIDE;
		case simplechess::CastlingRight::BlackKingSide:
			return SIMPLE_CHESS_CASTLING_RIGHT_BLACK_KING_SIDE;
		case simplechess::CastlingRight::BlackQueenSide:
			return SIMPLE_CHESS_CASTLING_RIGHT_BLACK_QUEEN_SIDE;
	}
	// Suppress warning
	return SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE;
}

uint8_t conversion_utils::c_castling_rights(uint8_t rights) {
	uint8_t result{0};

	if (rights & simplechess::CastlingRight::WhiteKingSide)
		result |= SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE;
	if (rights & simplechess::CastlingRight::WhiteQueenSide)
		result |= SIMPLE_CHESS_CASTLING_RIGHT_WHITE_QUEEN_SIDE;
	if (rights & simplechess::CastlingRight::BlackKingSide)
		result |= SIMPLE_CHESS_CASTLING_RIGHT_BLACK_KING_SIDE;
	if (rights & simplechess::CastlingRight::BlackQueenSide)
		result |= SIMPLE_CHESS_CASTLING_RIGHT_BLACK_QUEEN_SIDE;

	return result;
}

simple_chess_board_t conversion_utils::c_board(const simplechess::Board& board) {
	simple_chess_board_t result;
	for (int i = 0; i < 64; ++i) {
		uint8_t row = 1 + (i / 8);
		char col = 'a' + (i % 8);
		const auto square = simplechess::Square::fromRankAndFile(row, col);

		const auto piece = board.pieceAt(square);
		result.squares[i] = piece
			? simple_chess_square_content_from_piece(c_piece(*piece))
			: SIMPLE_CHESS_SQUARE_EMPTY;
	}
	return result;
}

simple_chess_game_stage_t conversion_utils::c_game_stage(const simplechess::GameStage& stage) {
	simple_chess_game_stage_t result;
	result.board = c_board(stage.board());
	result.active_color = c_color(stage.activeColor());
	result.castling_rights = c_castling_rights(stage.castlingRights());
	result.half_moves_since_last_capture_or_pawn_advance = stage.halfMovesSinceLastCaptureOrPawnAdvance();
	result.full_moves = stage.fullMoveCounter();
	result.has_en_passant_target = stage.enPassantTarget().has_value();
	if (result.has_en_passant_target) {
		result.en_passant_target = c_square(stage.enPassantTarget().value());
	}
	result.check_status = c_check_type(stage.checkStatus());
	strncpy(result.fen, stage.fen().c_str(), sizeof(result.fen) - 1);
	result.fen[sizeof(result.fen) - 1] = '\0';
	return result;
}

simple_chess_game_state_t conversion_utils::c_game_state(simplechess::GameState state) {
	switch(state) {
		case simplechess::GameState::Playing:
			return SIMPLE_CHESS_GAME_STATE_PLAYING;
		case simplechess::GameState::Drawn:
			return SIMPLE_CHESS_GAME_STATE_DRAWN;
		case simplechess::GameState::WhiteWon:
			return SIMPLE_CHESS_GAME_STATE_WHITE_WON;
		case simplechess::GameState::BlackWon:
			return SIMPLE_CHESS_GAME_STATE_BLACK_WON;
	}

	// Suppress warning
	return SIMPLE_CHESS_GAME_STATE_PLAYING;
}

simple_chess_draw_reason_t conversion_utils::c_draw_reason(simplechess::DrawReason reason) {
	switch (reason) {
		case simplechess::DrawReason::StaleMate:
			return SIMPLE_CHESS_DRAW_REASON_STALEMATE;
		case simplechess::DrawReason::InsufficientMaterial:
			return SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL;
		case simplechess::DrawReason::OfferedAndAccepted:
			return SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED;
		case simplechess::DrawReason::ThreeFoldRepetition:
			return SIMPLE_CHESS_DRAW_REASON_THREEFOLD_REPETITION;
		case simplechess::DrawReason::FiveFoldRepetition:
			return SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION;
		case simplechess::DrawReason::FiftyMoveRule:
			return SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE;
		case simplechess::DrawReason::SeventyFiveMoveRule:
			return SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE;
		case simplechess::DrawReason::OpponentInsufficientMaterial:
			return SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL;
	}

	// Suppress warning
	return SIMPLE_CHESS_DRAW_REASON_STALEMATE;
}

simplechess::DrawEnforcement conversion_utils::cpp_draw_enforcement(simple_chess_draw_enforcement_t enforcement) {
	switch (enforcement) {
		case SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC:
			return simplechess::DrawEnforcement::Automatic;
		case SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY:
			return simplechess::DrawEnforcement::ClaimOnly;
	}

	// Suppress warning
	return simplechess::DrawEnforcement::Automatic;
}

simple_chess_draw_enforcement_t conversion_utils::c_draw_enforcement(simplechess::DrawEnforcement enforcement) {
	switch (enforcement) {
		case simplechess::DrawEnforcement::Automatic:
			return SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC;
		case simplechess::DrawEnforcement::ClaimOnly:
			return SIMPLE_CHESS_DRAW_ENFORCEMENT_CLAIM_ONLY;
	}

	// Suppress warning
	return SIMPLE_CHESS_DRAW_ENFORCEMENT_AUTOMATIC;
}

// C to C++ conversions
simplechess::Color conversion_utils::cpp_color(simple_chess_color_t color) {
	switch (color) {
		case SIMPLE_CHESS_COLOR_WHITE:
			return simplechess::Color::White;
		case SIMPLE_CHESS_COLOR_BLACK:
			return simplechess::Color::Black;
	}

	// Suppress warning
	return simplechess::Color::White;
}

simplechess::Square conversion_utils::cpp_square(const simple_chess_square_t& square) {
	return simplechess::Square::fromRankAndFile(
			std::clamp<uint8_t>(square.rank, 1, 8),
			std::clamp<char>(std::tolower(square.file), 'a', 'h'));
}

simplechess::PieceType conversion_utils::cpp_piece_type(simple_chess_piece_type_t type) {
	switch (type) {
		case SIMPLE_CHESS_PIECE_TYPE_PAWN:
			return simplechess::PieceType::Pawn;
		case SIMPLE_CHESS_PIECE_TYPE_ROOK:
			return simplechess::PieceType::Rook;
		case SIMPLE_CHESS_PIECE_TYPE_KNIGHT:
			return simplechess::PieceType::Knight;
		case SIMPLE_CHESS_PIECE_TYPE_BISHOP:
			return simplechess::PieceType::Bishop;
		case SIMPLE_CHESS_PIECE_TYPE_QUEEN:
			return simplechess::PieceType::Queen;
		case SIMPLE_CHESS_PIECE_TYPE_KING:
			return simplechess::PieceType::King;
	}

	// Suppress warning
	return simplechess::PieceType::Pawn;
}

simplechess::Piece conversion_utils::cpp_piece(const simple_chess_piece_t& piece) {
	return simplechess::Piece(cpp_piece_type(piece.type), cpp_color(piece.color));
}

simplechess::PieceMove conversion_utils::cpp_piece_move(const simple_chess_piece_move_t& move) {
	if (move.is_promotion) {
		return simplechess::PieceMove::pawnPromotion(
				cpp_piece(move.piece),
				cpp_square(move.src),
				cpp_square(move.dst),
				cpp_piece_type(move.promoted_to));
	} else {
		return simplechess::PieceMove::regularMove(
				cpp_piece(move.piece),
				cpp_square(move.src),
				cpp_square(move.dst));
	}
}

simplechess::CheckType conversion_utils::cpp_check_type(simple_chess_check_type_t check) {
	switch (check) {
		case SIMPLE_CHESS_CHECK_TYPE_NONE:
			return simplechess::CheckType::NoCheck;
		case SIMPLE_CHESS_CHECK_TYPE_CHECK:
			return simplechess::CheckType::Check;
		case SIMPLE_CHESS_CHECK_TYPE_CHECKMATE:
			return simplechess::CheckType::CheckMate;
	}

	// Suppress warning
	return simplechess::CheckType::NoCheck;
}

simplechess::PlayedMove conversion_utils::cpp_played_move(const simple_chess_played_move_t& move) {
	const auto pieceMove = cpp_piece_move(move.move);
	std::optional<simplechess::Piece> captured;
	if (move.is_capture) {
		captured = cpp_piece(move.captured_piece);
	}
	const bool drawOffered = move.offers_draw;
	const auto checkType = cpp_check_type(move.check_type);
	const std::string algebraic = move.in_algebraic_notation;

	return simplechess::PlayedMoveBuilder::build(
			pieceMove,
			captured,
			drawOffered,
			checkType,
			algebraic);
}

simplechess::CastlingRight conversion_utils::cpp_castling_right(simple_chess_castling_right_t right) {
	switch(right) {
		case SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE:
			return simplechess::CastlingRight::WhiteKingSide;
		case SIMPLE_CHESS_CASTLING_RIGHT_WHITE_QUEEN_SIDE:
			return simplechess::CastlingRight::WhiteQueenSide;
		case SIMPLE_CHESS_CASTLING_RIGHT_BLACK_KING_SIDE:
			return simplechess::CastlingRight::BlackKingSide;
		case SIMPLE_CHESS_CASTLING_RIGHT_BLACK_QUEEN_SIDE:
			return simplechess::CastlingRight::BlackQueenSide;
	}

	// Suppress warning
	return simplechess::CastlingRight::WhiteKingSide;
}

uint8_t conversion_utils::cpp_castling_rights(uint8_t rights) {

	uint8_t result = 0;
	if (rights & SIMPLE_CHESS_CASTLING_RIGHT_WHITE_KING_SIDE) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::WhiteKingSide);
	}
	if (rights & SIMPLE_CHESS_CASTLING_RIGHT_WHITE_QUEEN_SIDE) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::WhiteQueenSide);
	}
	if (rights & SIMPLE_CHESS_CASTLING_RIGHT_BLACK_KING_SIDE) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::BlackKingSide);
	}
	if (rights & SIMPLE_CHESS_CASTLING_RIGHT_BLACK_QUEEN_SIDE) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::BlackQueenSide);
	}
	return result;
}

simplechess::Board conversion_utils::cpp_board(const simple_chess_board_t& board) {
	std::map<simplechess::Square, simplechess::Piece> position;
	for (uint8_t index = 0; index < 64; ++index) {
		simple_chess_piece_t piece;
		if (!simple_chess_square_content_piece(board.squares[index], &piece))
			continue;

		simple_chess_square_t square;
		simple_chess_square_from_index(index, &square);

		position.insert({cpp_square(square), cpp_piece(piece)});
	}
	return simplechess::BoardBuilder::build(position);
}

simplechess::GameStage conversion_utils::cpp_game_stage(const simple_chess_game_stage_t& stage) {
	// The C representation already carries both the FEN and the check status,
	// so the stage can be rebuilt from them as-is. Going through
	// FenUtils::fromFenString instead would regenerate the FEN it was just
	// handed and re-derive the check status by generating moves - per stage,
	// on every call that crosses the C boundary.
	return simplechess::GameStageBuilder::buildFromKnownFen(
			stage.fen,
			cpp_check_type(stage.check_status));
}

simplechess::GameState conversion_utils::cpp_game_state(simple_chess_game_state_t state) {
	switch (state) {
		case SIMPLE_CHESS_GAME_STATE_PLAYING:
			return simplechess::GameState::Playing;
		case SIMPLE_CHESS_GAME_STATE_DRAWN:
			return simplechess::GameState::Drawn;
		case SIMPLE_CHESS_GAME_STATE_WHITE_WON:
			return simplechess::GameState::WhiteWon;
		case SIMPLE_CHESS_GAME_STATE_BLACK_WON:
			return simplechess::GameState::BlackWon;
	}

	// Suppress warning
	return simplechess::GameState::Playing;
}

simplechess::DrawReason conversion_utils::cpp_draw_reason(simple_chess_draw_reason_t reason) {
	switch (reason) {
		case SIMPLE_CHESS_DRAW_REASON_STALEMATE:
			return simplechess::DrawReason::StaleMate;
		case SIMPLE_CHESS_DRAW_REASON_INSUFFICIENT_MATERIAL:
			return simplechess::DrawReason::InsufficientMaterial;
		case SIMPLE_CHESS_DRAW_REASON_OFFERED_AND_ACCEPTED:
			return simplechess::DrawReason::OfferedAndAccepted;
		case SIMPLE_CHESS_DRAW_REASON_THREEFOLD_REPETITION:
			return simplechess::DrawReason::ThreeFoldRepetition;
		case SIMPLE_CHESS_DRAW_REASON_FIVEFOLD_REPETITION:
			return simplechess::DrawReason::FiveFoldRepetition;
		case SIMPLE_CHESS_DRAW_REASON_FIFTY_MOVE_RULE:
			return simplechess::DrawReason::FiftyMoveRule;
		case SIMPLE_CHESS_DRAW_REASON_SEVENTY_FIVE_MOVE_RULE:
			return simplechess::DrawReason::SeventyFiveMoveRule;
		case SIMPLE_CHESS_DRAW_REASON_OPPONENT_INSUFFICIENT_MATERIAL:
			return simplechess::DrawReason::OpponentInsufficientMaterial;
	}

	// Suppress warning
	return simplechess::DrawReason::StaleMate;
}
