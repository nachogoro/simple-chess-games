#include <simplechess/SimpleChess.h>

#include "Builders.h"
#include "details/BoardAnalyzer.h"
#include "details/GameStageUpdater.h"
#include "details/GameStateDetector.h"
#include "details/fen/FenParser.h"
#include "details/fen/FenUtils.h"

#include <algorithm>

using namespace simplechess;

namespace internal
{
	/**
	 * Moves are generated one source square at a time, so they come out in
	 * whatever order move generation happens to visit the board in. The
	 * public interface promises them ordered by PieceMove::operator<
	 * instead, so they are sorted here - once per move actually played,
	 * rather than on every position examined while working out what those
	 * moves are.
	 *
	 * Duplicates are impossible (see MoveValidator::allAvailableMoves), so
	 * there is nothing to remove afterwards.
	 */
	std::vector<PieceMove> orderedMoves(std::vector<PieceMove> moves)
	{
		std::sort(moves.begin(), moves.end());
		return moves;
	}

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

	/**
	 * Returns the repetition bookkeeping for a game which has just played \p
	 * move from \p previousStage, given the bookkeeping \p previous of the
	 * game before the move.
	 *
	 * The map counts the positions of every stage in the game's history, so
	 * moving on adds the stage the move was played from. Rebuilding it from
	 * the history on every move would mean re-splitting every FEN string ever
	 * produced, making a move cost more the longer the game gets.
	 *
	 * A move which resets the halfmove clock or gives up a castling right
	 * makes every earlier position permanently unreachable - the material,
	 * the pawn structure or the castling rights can never be restored - so
	 * the accumulated counts can simply be dropped.
	 */
	std::map<std::string, uint8_t> updatedReachedPositions(
			const std::map<std::string, uint8_t>& previous,
			const GameStage& previousStage,
			const GameStage& nextStage)
	{
		const bool everyEarlierPositionIsUnreachable
			= nextStage.halfMovesSinceLastCaptureOrPawnAdvance() == 0
				|| nextStage.castlingRights() != previousStage.castlingRights();

		if (everyEarlierPositionIsUnreachable)
		{
			return {};
		}

		std::map<std::string, uint8_t> result = previous;
		++result[details::FenUtils::fenForRepetitions(previousStage.fen())];
		return result;
	}
}

Game simplechess::createNewGame(const DrawEnforcement drawEnforcement)
{
	const std::string fenOfInitialPosition
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	return createGameFromFen(fenOfInitialPosition, drawEnforcement);
}

Game simplechess::createGameFromFen(
		const std::string& fen,
		const DrawEnforcement drawEnforcement)
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
			= details::GameStateDetector::detect(currentStage, false, {}, drawEnforcement);

		return GameBuilder::build(
				information.gameState,
				information.reasonItWasDrawn,
				{},
				{currentStage},
				internal::orderedMoves(information.availableMoves),
				information.reasonToClaimDraw,
				drawEnforcement);
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
		= details::GameStateDetector::detect(originalStage, false, {}, drawEnforcement);

	const Game originalGame = GameBuilder::build(
		information.gameState,
		information.reasonItWasDrawn,
		{}, // empty history
		originalStage,
		internal::orderedMoves(information.availableMoves),
		information.reasonToClaimDraw,
		drawEnforcement);

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

	const std::vector<PieceMove>& validMoves = game.allAvailableMoves();

	if (std::find(validMoves.begin(), validMoves.end(), move) == validMoves.end())
	{
		throw IllegalStateException("Attempted to make invalid move");
	}

	const DrawEnforcement drawEnforcement = game.drawEnforcement();

	const details::UpdatedStage next
		= details::GameStageUpdater::makeMoveWithAnalysis(
				game.currentStage(),
				move);

	const details::GameStateInformation information
		= details::GameStateDetector::detect(
				next.stage,
				next.analysis,
				offerDraw,
				GameBuilder::previouslyReachedPositions(game),
				drawEnforcement);

	std::map<std::string, uint8_t> nextReachedPositions
		= internal::updatedReachedPositions(
				GameBuilder::previouslyReachedPositions(game),
				game.currentStage(),
				next.stage);

	auto nextHistory = game.history();
	nextHistory.reserve(nextHistory.size() + 1);
	nextHistory.push_back(
			{game.currentStage(),
			// The check status this move produces is, by definition, the
			// check status of the stage it leads to, which has just been
			// derived.
			PlayedMoveBuilder::build(
					game.currentStage().board(),
					move,
					offerDraw,
					next.stage.checkStatus())});

	return GameBuilder::build(
			information.gameState,
			information.reasonItWasDrawn,
			std::move(nextHistory),
			next.stage,
			internal::orderedMoves(information.availableMoves),
			information.reasonToClaimDraw,
			drawEnforcement,
			std::move(nextReachedPositions));
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
			{},
			game.drawEnforcement(),
			GameBuilder::previouslyReachedPositions(game));
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
			{},
			game.drawEnforcement(),
			GameBuilder::previouslyReachedPositions(game));
}
