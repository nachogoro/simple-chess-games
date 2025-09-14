#include "Builders.h"

#include "details/fen/FenUtils.h"
#include "details/AlgebraicNotationGenerator.h"
#include "details/BoardAnalyzer.h"
#include "details/MoveValidator.h"
#include "details/GameStageUpdater.h"

using namespace simplechess;

GameStage GameStageBuilder::build(
		const Board& board,
		const Color activeColor,
		const uint8_t castlingRights,
		const uint16_t halfmoveClock,
		const uint16_t fullmoveClock,
		const std::optional<PlayedMove>& move)
{
	const std::string fen = details::FenUtils::generateFen(
			board,
			activeColor,
			castlingRights,
			move
				? details::MoveValidator::enPassantTarget({move->pieceMove()})
				: std::nullopt,
			halfmoveClock,
			fullmoveClock);

	return GameStage(
			board,
			activeColor,
			castlingRights,
			halfmoveClock,
			fullmoveClock,
			fen,
			move);
}

Game GameBuilder::build(
		const GameState gameState,
		const std::optional<DrawReason>& drawReason,
		const std::vector<GameStage>& history,
		const std::set<PieceMove>& allAvailableMoves,
		const std::optional<DrawReason>& reasonToClaimDraw)
{
	return {
		gameState,
		drawReason,
		history,
		allAvailableMoves,
		reasonToClaimDraw };
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
		const std::set<PieceMove> availableResponses
			= details::MoveValidator::allAvailableMoves(
					afterMove,
					details::MoveValidator::enPassantTarget({move}),
					0, // If in check, we can't castle any way
					oppositeColor(move.piece().color()));

		checkType = (availableResponses.size() == 0)
			? CheckType::CheckMate
			: CheckType::Check;
	}

	return PlayedMove(
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
