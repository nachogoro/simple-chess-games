#include <cpp/simplechess/SimpleChess.h>

#include "Builders.h"
#include "details/BoardAnalyzer.h"
#include "details/GameStageUpdater.h"
#include "details/GameStateDetector.h"
#include "details/fen/FenParser.h"
#include "details/fen/FenUtils.h"

using namespace simplechess;

namespace internal
{
	void validateGamePosition(const Board& board, Color activeColor, uint8_t castlingRights)
	{
		// 1. Validate that there is exactly one King per side
		uint8_t whiteKings = 0;
		uint8_t blackKings = 0;

		for (const auto& entry : board.occupiedSquares())
		{
			if (entry.second.type() == PieceType::King)
			{
				if (entry.second.color() == Color::White)
				{
					whiteKings++;
				}
				else
				{
					blackKings++;
				}
			}
		}

		if (whiteKings != 1 || blackKings != 1)
		{
			throw std::invalid_argument("Invalid number of kings on board");
		}

		// 2. Validate that the color to move cannot be checking the opposite King
		if (details::BoardAnalyzer::isInCheck(board, oppositeColor(activeColor)))
		{
			throw std::invalid_argument("Color to move is already checking");
		}

		// 3. Validate castling rights consistency
		const Piece whiteKing = {PieceType::King, Color::White};
		const Piece blackKing = {PieceType::King, Color::Black};
		const Piece whiteRook = {PieceType::Rook, Color::White};
		const Piece blackRook = {PieceType::Rook, Color::Black};

		if ((castlingRights & static_cast<uint8_t>(CastlingRight::WhiteKingSide))
				&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
					|| *board.pieceAt(Square::fromString("h1")) != whiteRook))
		{
			throw std::invalid_argument(
					"Kingside castling right for white is inconsistent with board state");
		}

		if ((castlingRights & static_cast<uint8_t>(CastlingRight::WhiteQueenSide))
				&& (*board.pieceAt(Square::fromString("e1")) != whiteKing
					|| *board.pieceAt(Square::fromString("a1")) != whiteRook))
		{
			throw std::invalid_argument(
					"Queenside castling right for white is inconsistent with board state");
		}

		if ((castlingRights & static_cast<uint8_t>(CastlingRight::BlackKingSide))
				&& (*board.pieceAt(Square::fromString("e8")) != blackKing
					|| *board.pieceAt(Square::fromString("h8")) != blackRook))
		{
			throw std::invalid_argument(
					"Kingside castling right for black is inconsistent with board state");
		}

		if ((castlingRights & static_cast<uint8_t>(CastlingRight::BlackQueenSide))
				&& (*board.pieceAt(Square::fromString("e8")) != blackKing
					|| *board.pieceAt(Square::fromString("a8")) != blackRook))
		{
			throw std::invalid_argument(
					"Queenside castling right for black is inconsistent with board state");
		}
	}

	std::map<std::string, uint8_t> getPreviouslyReachedPositionsMap(
			const std::vector<std::pair<GameStage, PlayedMove>>& history)
	{
		std::map<std::string, uint8_t> result;

		for (const auto& [stage, move] : history)
		{
			const std::string repetitionFen
				= details::FenUtils::fenForRepetitions(stage.fen());

			if (result.count(repetitionFen))
			{
				result.at(repetitionFen)++;
			}
			else
			{
				result.insert({repetitionFen, 1});
			}
		}

		return result;
	}
}

Game simplechess::createNewGame()
{
	const std::string fenOfInitialPosition
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	return createGameFromFen(fenOfInitialPosition);
}

Game simplechess::createGameFromFen(const std::string& fen)
{
	const details::FenParser parsedState = details::FenParser::parse(fen);

	// We can infer the prior move if there is an en passant target
	const std::optional<Square> epSquare = parsedState.enPassantTarget();
	std::optional<PieceMove> lastMove;
	if (epSquare)
	{
		const Piece pawn(
				PieceType::Pawn,
				(epSquare->rank() == 3)
					? Color::White
					: Color::Black);

		const Square src = Square::fromRankAndFile(
				((pawn.color() == Color::White)
					? 2
					: 7),
				epSquare->file());

		const Square dst = Square::fromRankAndFile(
				((pawn.color() == Color::White)
					? 4
					: 5),
				epSquare->file());

		lastMove = { PieceMove::regularMove(pawn, src, dst) };
	}

	// Validate the parsed position
	internal::validateGamePosition(parsedState.board(), parsedState.activeColor(), parsedState.castlingRights());

	if (!lastMove)
	{
		const GameStage currentStage = GameStageBuilder::build(
			parsedState.board(),
			parsedState.activeColor(),
			parsedState.castlingRights(),
			parsedState.halfMovesSinceLastCaptureOrPawnAdvance(),
			parsedState.fullMoveCounter(),
			parsedState.enPassantTarget());

		const details::GameStateInformation information
			= details::GameStateDetector::detect(currentStage, false, {});

		return GameBuilder::build(
				information.gameState,
				information.reasonItWasDrawn,
				{},
				{currentStage},
				information.availableMoves,
				information.reasonToClaimDraw);
	}

	// We can infer the last move, so we want to start the history of the game
	// one stage sooner.
	// We get the original board state by making a backwards pawn move.
	const Board originalBoardState =
		details::BoardAnalyzer::makeMoveOnBoard(
				parsedState.board(),
				PieceMove::regularMove(
					lastMove->piece(),
					lastMove->dst(),
					lastMove->src()));

	// Validate the original position too
	internal::validateGamePosition(originalBoardState, oppositeColor(parsedState.activeColor()), parsedState.castlingRights());

	const uint16_t fullMoveCounterDecrease
		= (parsedState.activeColor() == Color::White)
			? 1
			: 0;

	const GameStage originalStage = GameStageBuilder::build(
		originalBoardState,
		oppositeColor(parsedState.activeColor()),
		parsedState.castlingRights(),
		0, // Reset halfmove clock
		static_cast<uint16_t>(parsedState.fullMoveCounter() - fullMoveCounterDecrease),
		std::nullopt); // No en passant target

	const details::GameStateInformation information
		= details::GameStateDetector::detect(originalStage, false, {});

	const Game originalGame = GameBuilder::build(
		information.gameState,
		information.reasonItWasDrawn,
		{}, // empty history
		originalStage,
		information.availableMoves,
		information.reasonToClaimDraw);

	return makeMove(originalGame, *lastMove, false);
}

Game simplechess::makeMove(
		const Game& game,
		const PieceMove& move,
		bool offerDraw)
{
	if (game.gameState() != GameState::Playing)
	{
		throw IllegalStateException("Attempted to make a move in finished game");
	}

	const Square& src = move.src();
	const std::set<PieceMove> validMoves = game.allAvailableMoves();

	if (validMoves.count(move) == 0)
	{
		throw IllegalStateException("Attempted to make invalid move");
	}

	const GameStage nextStage = details::GameStageUpdater::makeMove(
			game.currentStage(),
			move,
			offerDraw);

	const details::GameStateInformation information
		= details::GameStateDetector::detect(
				nextStage,
				offerDraw,
				internal::getPreviouslyReachedPositionsMap(game.history()));

	auto nextHistory = game.history();
	nextHistory.push_back(
			{game.currentStage(),
			PlayedMoveBuilder::build(
					game.currentStage().board(),
					move,
					offerDraw)});

	return GameBuilder::build(
			information.gameState,
			information.reasonItWasDrawn,
			nextHistory,
			nextStage,
			information.availableMoves,
			information.reasonToClaimDraw);
}

Game simplechess::claimDraw(const Game& game)
{
	if (game.gameState() != GameState::Playing)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	const std::optional<DrawReason> reason = game.reasonToClaimDraw();

	if (!reason)
	{
		throw IllegalStateException(
				"Attempted to claim draw when it wasn't allowed");
	}

	return GameBuilder::build(
			GameState::Drawn,
			reason,
			game.history(),
			game.currentStage(),
			{},
			{});
}

Game simplechess::resign(const Game& game, Color resigningPlayer)
{
	if (game.gameState() != GameState::Playing)
	{
		throw IllegalStateException("Cannot resign finished games");
	}

	return GameBuilder::build(
			(resigningPlayer == Color::White)
				? GameState::BlackWon
				: GameState::WhiteWon,
			{},
			game.history(),
			game.currentStage(),
			{},
			{});
}
