#include "conversion_utils.h"
#include "cpp/simplechess/SimpleChess.h"
#include "../core/Builders.h"
#include "../core/details/fen/FenUtils.h"
#include <cstring>

// C++ to C conversions
color_t conversion_utils::c_color(simplechess::Color color) {
	switch (color) {
		case simplechess::Color::White:
			return ColorWhite;
		case simplechess::Color::Black:
			return ColorBlack;
	}

	// Supress warning
	return ColorWhite;
}

square_t conversion_utils::c_square(const simplechess::Square& square) {
	return {square.rank(), square.file()};
}

piece_type_t conversion_utils::c_piece_type(simplechess::PieceType type) {
	switch (type) {
		case simplechess::PieceType::Pawn:
			return PieceTypePawn;
		case simplechess::PieceType::Rook:
			return PieceTypeRook;
		case simplechess::PieceType::Knight:
			return PieceTypeKnight;
		case simplechess::PieceType::Bishop:
			return PieceTypeBishop;
		case simplechess::PieceType::Queen:
			return PieceTypeQueen;
		case simplechess::PieceType::King:
			return PieceTypeKing;
	}

	// Suppress warning
	return PieceTypePawn;
}

piece_t conversion_utils::c_piece(const simplechess::Piece& piece) {
	return {c_piece_type(piece.type()), c_color(piece.color())};
}

piece_move_t conversion_utils::c_piece_move(const simplechess::PieceMove& move) {
	return {
		c_piece(move.piece()),
		c_square(move.src()),
		c_square(move.dst()),
		move.promoted().has_value(),
		(move.promoted().has_value() ? c_piece_type(*move.promoted())
		 : PieceTypePawn)
	};
}

check_type_t conversion_utils::c_check_type(simplechess::CheckType check)
{
	switch (check) {
		case simplechess::CheckType::NoCheck:
			return CheckTypeNone;
		case simplechess::CheckType::Check:
			return CheckTypeCheck;
		case simplechess::CheckType::CheckMate:
			return CheckTypeCheckMate;
	}

	// Suppress warning
	return CheckTypeNone;
};

played_move_t conversion_utils::c_played_move(const simplechess::PlayedMove& move) {
	played_move_t result;
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

castling_right_t conversion_utils::c_castling_rights(simplechess::CastlingRight right) {
	switch (right) {
		case simplechess::CastlingRight::WhiteKingSide:
			return CastlingRightWhiteKingSide;
		case simplechess::CastlingRight::WhiteQueenSide:
			return CastlingRightWhiteQueenSide;
		case simplechess::CastlingRight::BlackKingSide:
			return CastlingRightBlackKingSide;
		case simplechess::CastlingRight::BlackQueenSide:
			return CastlingRightBlackQueenSide;
	}
	// Suppress warning
	return CastlingRightWhiteKingSide;
}

uint8_t conversion_utils::c_castling_rights(uint8_t rights) {
	uint8_t result{0};

	if (rights & simplechess::CastlingRight::WhiteKingSide)
		result |= CastlingRightWhiteKingSide;
	if (rights & simplechess::CastlingRight::WhiteQueenSide)
		result |= CastlingRightWhiteQueenSide;
	if (rights & simplechess::CastlingRight::BlackKingSide)
		result |= CastlingRightBlackKingSide;
	if (rights & simplechess::CastlingRight::BlackQueenSide)
		result |= CastlingRightBlackQueenSide;

	return result;
}

board_t conversion_utils::c_board(const simplechess::Board& board) {
	board_t result;
	for (int i = 0; i < 64; ++i) {
		uint8_t row = 1 + (i / 8);
		char col = 'a' + (i % 8);
		const auto square = simplechess::Square::fromRankAndFile(row, col);

		const auto piece = board.pieceAt(square);
		result.occupied[i] = piece.has_value();
		if (piece) {
			result.piece_at[i] = c_piece(*piece);
		}
	}
	return result;
}

game_stage_t conversion_utils::c_game_stage(const simplechess::GameStage& stage) {
	game_stage_t result;
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

game_state_t conversion_utils::c_game_state(simplechess::GameState state) {
	switch(state) {
		case simplechess::GameState::Playing:
			return GameStatePlaying;
		case simplechess::GameState::Drawn:
			return GameStateDrawn;
		case simplechess::GameState::WhiteWon:
			return GameStateWhiteWon;
		case simplechess::GameState::BlackWon:
			return GameStateBlackWon;
	}

	// Suppress warning
	return GameStatePlaying;
}

draw_reason_t conversion_utils::c_draw_reason(simplechess::DrawReason reason) {
	switch (reason) {
		case simplechess::DrawReason::StaleMate:
			return DrawReasonStaleMate;
		case simplechess::DrawReason::InsufficientMaterial:
			return DrawReasonInsufficientMaterial;
		case simplechess::DrawReason::OfferedAndAccepted:
			return DrawReasonOfferedAndAccepted;
		case simplechess::DrawReason::ThreeFoldRepetition:
			return DrawReasonThreeFoldRepetition;
		case simplechess::DrawReason::FiveFoldRepetition:
			return DrawReasonFiveFoldRepetition;
		case simplechess::DrawReason::FiftyMoveRule:
			return DrawReasonFiftyMoveRule;
		case simplechess::DrawReason::SeventyFiveMoveRule:
			return DrawReasonSeventyFiveMoveRule;
	}

	// Suppress warning
	return DrawReasonStaleMate;
}

game_t* conversion_utils::c_game(const simplechess::Game& game) {
	game_t* result = new game_t();
	result->state = c_game_state(game.gameState());
	if (game.gameState() == simplechess::GameState::Drawn)
		result->draw_reason = c_draw_reason(game.drawReason());

	result->history_size = static_cast<uint16_t>(game.history().size());
	if (result->history_size)
		result->history = new game_history_entry_t[result->history_size];

	for (uint16_t i = 0; i < result->history_size; ++i) {
		strncpy(result->history[i].fen, game.history()[i].first.fen().c_str(), sizeof(result->history[i].fen) - 1);
		result->history[i].fen[sizeof(result->history[i].fen) - 1] = '\0';
		result->history[i].played_move = c_played_move(game.history()[i].second);
	}

	result->available_move_count = static_cast<uint16_t>(game.allAvailableMoves().size());
	if (result->available_move_count)
		result->available_moves = new piece_move_t[result->available_move_count];

	uint16_t i = 0;
	for (const auto& pieceMove : game.allAvailableMoves()) {
		result->available_moves[i] = c_piece_move(pieceMove);
		++i;
	}

	result->current_stage = c_game_stage(game.currentStage());
	if (game.gameState() == simplechess::GameState::Playing) {
		result->is_draw_claimable = game.reasonToClaimDraw().has_value();
		if (result->is_draw_claimable) {
			result->reason_to_claim_draw = c_draw_reason(*game.reasonToClaimDraw());
		}
	}

	return result;
}

// C to C++ conversions
simplechess::Color conversion_utils::cpp_color(color_t color) {
	switch (color) {
		case ColorWhite:
			return simplechess::Color::White;
		case ColorBlack:
			return simplechess::Color::Black;
	}

	// Suppress warning
	return simplechess::Color::White;
}

simplechess::Square conversion_utils::cpp_square(const square_t& square) {
	return simplechess::Square::fromRankAndFile(
			std::clamp<uint8_t>(square.rank, 1, 8),
			std::clamp<char>(std::tolower(square.file), 'a', 'h'));
}

simplechess::PieceType conversion_utils::cpp_piece_type(piece_type_t type) {
	switch (type) {
		case PieceTypePawn:
			return simplechess::PieceType::Pawn;
		case PieceTypeRook:
			return simplechess::PieceType::Rook;
		case PieceTypeKnight:
			return simplechess::PieceType::Knight;
		case PieceTypeBishop:
			return simplechess::PieceType::Bishop;
		case PieceTypeQueen:
			return simplechess::PieceType::Queen;
		case PieceTypeKing:
			return simplechess::PieceType::King;
	}

	// Suppress warning
	return simplechess::PieceType::Pawn;
}

simplechess::Piece conversion_utils::cpp_piece(const piece_t& piece) {
	return simplechess::Piece(cpp_piece_type(piece.type), cpp_color(piece.color));
}

simplechess::PieceMove conversion_utils::cpp_piece_move(const piece_move_t& move) {
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

simplechess::CheckType conversion_utils::cpp_check_type(check_type_t check) {
	switch (check) {
		case CheckTypeNone:
			return simplechess::CheckType::NoCheck;
		case CheckTypeCheck:
			return simplechess::CheckType::Check;
		case CheckTypeCheckMate:
			return simplechess::CheckType::CheckMate;
	}

	// Suppress warning
	return simplechess::CheckType::NoCheck;
}

simplechess::PlayedMove conversion_utils::cpp_played_move(const played_move_t& move) {
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

simplechess::CastlingRight conversion_utils::cpp_castling_right(castling_right_t right) {
	switch(right) {
		case CastlingRightWhiteKingSide:
			return simplechess::CastlingRight::WhiteKingSide;
		case CastlingRightWhiteQueenSide:
			return simplechess::CastlingRight::WhiteQueenSide;
		case CastlingRightBlackKingSide:
			return simplechess::CastlingRight::BlackKingSide;
		case CastlingRightBlackQueenSide:
			return simplechess::CastlingRight::BlackQueenSide;
	}

	// Suppress warning
	return simplechess::CastlingRight::WhiteKingSide;
}

uint8_t conversion_utils::cpp_castling_rights(uint8_t rights) {

	uint8_t result = 0;
	if (rights & CastlingRightWhiteKingSide) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::WhiteKingSide);
	}
	if (rights & CastlingRightWhiteQueenSide) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::WhiteQueenSide);
	}
	if (rights & CastlingRightBlackKingSide) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::BlackKingSide);
	}
	if (rights & CastlingRightBlackQueenSide) {
		result |= static_cast<uint8_t>(simplechess::CastlingRight::BlackQueenSide);
	}
	return result;
}

simplechess::Board conversion_utils::cpp_board(const board_t& board) {
	std::map<simplechess::Square, simplechess::Piece> position;
	for (uint8_t index = 0; index < 64; ++index) {
		if (!board.occupied[index]) continue;
		position.insert({
				cpp_square(simple_chess_square_from_index(index)),
				cpp_piece(board.piece_at[index])
				});
	}
	return simplechess::BoardBuilder::build(position);
}

simplechess::GameStage conversion_utils::cpp_game_stage(const game_stage_t& stage) {
	return simplechess::details::FenUtils::fromFenString(stage.fen);
}

simplechess::GameState conversion_utils::cpp_game_state(game_state_t state) {
	switch (state) {
		case GameStatePlaying:
			return simplechess::GameState::Playing;
		case GameStateDrawn:
			return simplechess::GameState::Drawn;
		case GameStateWhiteWon:
			return simplechess::GameState::WhiteWon;
		case GameStateBlackWon:
			return simplechess::GameState::BlackWon;
	}

	// Suppress warning
	return simplechess::GameState::Playing;
}

simplechess::DrawReason conversion_utils::cpp_draw_reason(draw_reason_t reason) {
	switch (reason) {
		case DrawReasonStaleMate:
			return simplechess::DrawReason::StaleMate;
		case DrawReasonInsufficientMaterial:
			return simplechess::DrawReason::InsufficientMaterial;
		case DrawReasonOfferedAndAccepted:
			return simplechess::DrawReason::OfferedAndAccepted;
		case DrawReasonThreeFoldRepetition:
			return simplechess::DrawReason::ThreeFoldRepetition;
		case DrawReasonFiveFoldRepetition:
			return simplechess::DrawReason::FiveFoldRepetition;
		case DrawReasonFiftyMoveRule:
			return simplechess::DrawReason::FiftyMoveRule;
		case DrawReasonSeventyFiveMoveRule:
			return simplechess::DrawReason::SeventyFiveMoveRule;
	}

	// Suppress warning
	return simplechess::DrawReason::StaleMate;
}

simplechess::Game conversion_utils::cpp_game(const game_t& game) {
	const auto state = cpp_game_state(game.state);

	const std::optional<simplechess::DrawReason> drawReason = (state == simplechess::GameState::Drawn)
		? std::make_optional(cpp_draw_reason(game.draw_reason))
		: std::nullopt;

	std::vector<std::pair<simplechess::GameStage, simplechess::PlayedMove>> history;
	for (uint16_t index = 0; index < game.history_size; ++index) {
		history.push_back({
				simplechess::details::FenUtils::fromFenString(game.history[index].fen),
				cpp_played_move(game.history[index].played_move)});
	}

	simplechess::GameStage currentStage = cpp_game_stage(game.current_stage);
	std::set<simplechess::PieceMove> allAvailableMoves;
	for (uint16_t index = 0; index < game.available_move_count; ++index) {
		allAvailableMoves.insert(cpp_piece_move(game.available_moves[index]));
	}

	std::optional<simplechess::DrawReason> reasonToClaimDraw;
	if (game.is_draw_claimable) {
		reasonToClaimDraw = cpp_draw_reason(game.reason_to_claim_draw);
	}

	return simplechess::GameBuilder::build(
			state,
			drawReason,
			history,
			currentStage,
			allAvailableMoves,
			reasonToClaimDraw);
}
