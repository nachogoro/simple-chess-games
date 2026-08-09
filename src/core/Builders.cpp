#include "Builders.h"

#include "details/fen/FenParser.h"
#include "details/fen/FenUtils.h"
#include "details/AlgebraicNotationGenerator.h"
#include "details/BoardAnalyzer.h"
#include "details/MoveValidator.h"

using namespace simplechess;

GameStage GameStageBuilder::build(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const std::optional<Square>& enPassantTarget)
{
	// Calculate check status. Generating the legal moves is only needed to
	// tell check from checkmate, so it is skipped when the king is safe.
	const bool isInCheck = details::BoardAnalyzer::isInCheck(board, activeColor);
	CheckType checkStatus = CheckType::NoCheck;
	if (isInCheck) {
		const std::vector<PieceMove> availableMoves = details::MoveValidator::allAvailableMoves(
			board,
			enPassantTarget,
			castlingRights,
			activeColor);
		checkStatus = (availableMoves.empty()) ? CheckType::CheckMate : CheckType::Check;
	}

	return build(
		board,
		activeColor,
		castlingRights,
		halfmoveClock,
		fullmoveClock,
		enPassantTarget,
		checkStatus);
}

GameStage GameStageBuilder::build(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const std::optional<Square>& enPassantTarget,
		const CheckType checkStatus)
{
	// Generate FEN string
	const std::string fen = details::FenUtils::generateFen(
		board,
		activeColor,
		castlingRights,
		enPassantTarget,
		halfmoveClock,
		fullmoveClock);

	return GameStage(
		board,
		activeColor,
		castlingRights,
		halfmoveClock,
		fullmoveClock,
		fen,
		enPassantTarget,
		checkStatus);
}

GameStage GameStageBuilder::buildFromKnownFen(
		const std::string& fen,
		const CheckType checkStatus)
{
	const details::FenParser parsed = details::FenParser::parse(fen);

	return GameStage(
		parsed.board(),
		parsed.activeColor(),
		parsed.castlingRights(),
		parsed.halfMovesSinceLastCaptureOrPawnAdvance(),
		parsed.fullMoveCounter(),
		fen,
		parsed.enPassantTarget(),
		checkStatus);
}

Game GameBuilder::build(
		const GameState gameState,
		const std::optional<DrawReason>& drawReason,
		std::vector<std::pair<GameStage, PlayedMove>> history,
		GameStage currentStage,
		std::set<PieceMove> allAvailableMoves,
		const std::optional<DrawReason>& reasonToClaimDraw,
		const DrawEnforcement drawEnforcement,
		std::map<std::string, uint8_t> previouslyReachedPositions)
{
	return {
		gameState,
		drawReason,
		std::move(history),
		std::move(currentStage),
		std::move(allAvailableMoves),
		reasonToClaimDraw,
		drawEnforcement,
		std::move(previouslyReachedPositions) };
}

const std::map<std::string, uint8_t>& GameBuilder::previouslyReachedPositions(
		const Game& game)
{
	return game.previouslyReachedPositions();
}

Board BoardBuilder::build(
		const std::map<Square, Piece> positions)
{
	return {positions};
}

PlayedMove PlayedMoveBuilder::build(
		const Board& board,
		const PieceMove& move,
		const bool drawOffered)
{
	const Board afterMove
		= details::BoardAnalyzer::makeMoveOnBoard(
				board,
				move);

	const bool isInCheck
		= details::BoardAnalyzer::isInCheck(
				afterMove,
				oppositeColor(move.piece().color()));

	CheckType checkType;

	if (!isInCheck)
	{
		checkType = CheckType::NoCheck;
	}
	else
	{
		const std::vector<PieceMove> availableResponses
			= details::MoveValidator::allAvailableMoves(
					afterMove,
					details::MoveValidator::enPassantTarget(afterMove, {move}),
					0, // If in check, we can't castle any way
					oppositeColor(move.piece().color()));

		checkType = (availableResponses.size() == 0)
			? CheckType::CheckMate
			: CheckType::Check;
	}

	return build(board, move, drawOffered, checkType);
}

PlayedMove PlayedMoveBuilder::build(
		const Board& board,
		const PieceMove& move,
		const bool drawOffered,
		const CheckType checkType)
{
	return PlayedMoveBuilder::build(
			move,
			board.pieceAt(move.dst()),
			drawOffered,
			checkType,
			details::AlgebraicNotationGenerator::toAlgebraicNotation(
				board,
				move,
				drawOffered,
				checkType));
}

PlayedMove PlayedMoveBuilder::build(
		const PieceMove& pieceMove,
		const std::optional<Piece>& capturedPiece,
		bool drawOffered,
		CheckType checkType,
		const std::string& algebraicNotation)
{
	return {
		pieceMove,
		capturedPiece,
		drawOffered,
		checkType,
		algebraicNotation
	};
}
