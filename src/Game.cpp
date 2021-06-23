#include <Game.h>
#include "details/BoardAnalyzer.h"
#include "details/DrawEvaluator.h"
#include "details/MoveValidator.h"
#include "details/fen/FenParser.h"
#include "details/fen/FenUtils.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/tuple/tuple.hpp>

#include <algorithm>

using namespace simplechess;

namespace internal
{
	boost::tuple<GameState, boost::optional<DrawReason>> inferGameStateFromStage(
			const GameStage& stage,
			const std::map<std::string, uint8_t>& previouslyReachedPositions)
	{
		const bool inCheck
			= details::BoardAnalyzer::isInCheck(
					stage.board(),
					stage.activeColor());

		if (inCheck)
		{
			const std::set<PieceMove> allPossibleMoves
				= details::MoveValidator::allAvailableMoves(
						stage.board(),
						stage.move()
						? details::MoveValidator::enPassantTarget(stage.move()->pieceMove())
						: boost::optional<Square>() ,
						stage.castlingRights(),
						stage.activeColor());

			if (allPossibleMoves.size() == 0)
			{
				// If the active color can't move and is in check, it is check
				// mate
				return {
					stage.activeColor() == COLOR_WHITE
						? GAME_STATE_BLACK_WON
						: GAME_STATE_WHITE_WON,
						{} };
			}
		}

		const boost::optional<DrawReason> reason
			= details::DrawEvaluator::reasonToDraw(
					stage,
					previouslyReachedPositions);

		if (!reason
				|| (*reason != DRAW_REASON_STALEMATE
						&& *reason != DRAW_REASON_75_MOVE_RULE
						&& *reason != DRAW_REASON_FIVE_FOLD_REPETITION
						&& *reason != DRAW_REASON_INSUFFICIENT_MATERIAL))
		{
			// If there is a reason to draw, it is not automatically applied,
			// so the game is still playable
			return {GAME_STATE_PLAYING, {}};
		}

		// There is a mandatory draw reason, so the game is drawn
		return { GAME_STATE_DRAWN, reason };
	}

	std::map<std::string, uint8_t> getPositionsMap(
			const std::vector<GameStage>& history)
	{
		std::map<std::string, uint8_t> result;

		for (const auto& stage : history)
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

Game Game::createNewGame() {
	const std::string fenOfInitialPosition
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	return createGameFromStartingFen(fenOfInitialPosition);
}

Game Game::createGameFromStartingFen(const std::string& fen)
{
	const details::FenParser parsedState = details::FenParser::parse(fen);

	// We can infer the prior move if there is an en passant target
	const boost::optional<Square> epSquare = parsedState.enPassantTarget();
	boost::optional<PieceMove> lastMove;
	if (epSquare)
	{
		const Piece pawn(
				TYPE_PAWN,
				(epSquare->rank() == 3)
					? COLOR_WHITE
					: COLOR_BLACK);

		const Square src = Square::instantiateWithRankAndFile(
				((pawn.color() == COLOR_WHITE)
					? 2
					: 7),
				epSquare->file());

		const Square dst = Square::instantiateWithRankAndFile(
				((pawn.color() == COLOR_WHITE)
					? 4
					: 5),
				epSquare->file());

		lastMove = { PieceMove::regularMove(pawn, src, dst) };
	}

	if (!lastMove)
	{
		const GameStage currentStage = {
			parsedState.board(),
			parsedState.activeColor(),
			parsedState.castlingRights(),
			parsedState.halfMovesSinceLastCaptureOrPawnAdvance(),
			parsedState.fullMoveCounter(),
			{}};

		const boost::tuple<GameState, boost::optional<DrawReason>> fullState =
				internal::inferGameStateFromStage(currentStage, {});

		return Game(
				fullState.get<0>(),
				fullState.get<1>(),
				{currentStage});
	}

	// We can infer the last move, so we want to start the history of the game
	// one stage sooner.
	// We get the original board state by making a backwards pawn move.
	const Board originalBoardState = parsedState.board().makeMove(
			PieceMove::regularMove(
				lastMove->piece(),
				lastMove->dst(),
				lastMove->src()));

	const uint16_t fullMoveCounterDecrease
		= (parsedState.activeColor() == COLOR_WHITE)
			? 1
			: 0;

	const GameStage originalStage = {
		originalBoardState,
		oppositeColor(parsedState.activeColor()),
		parsedState.castlingRights(),
		0, // Unknown, filler value
		static_cast<uint16_t>(
				parsedState.fullMoveCounter() - fullMoveCounterDecrease),
		{} };

	const boost::tuple<GameState, boost::optional<DrawReason>> fullState =
		internal::inferGameStateFromStage(originalStage, {});

	const Game originalGame(
				fullState.get<0>(),
				fullState.get<1>(),
				{originalStage});

	return originalGame.makeMove(*lastMove, false);
}

Game::Game(
		const GameState gameState,
		const boost::optional<DrawReason> drawReason,
		const std::vector<GameStage>& history)
	: mGameState(gameState),
	  mDrawReason(drawReason),
	  mHistory(history),
	  mTimesPositionsReached(internal::getPositionsMap(history))
{
	if ((gameState == GAME_STATE_DRAWN && !drawReason)
			|| (gameState != GAME_STATE_DRAWN && drawReason))
	{
		throw std::invalid_argument(
				"Inconsistent arguments related to draw reason");
	}
}

const GameStage& Game::currentStage() const
{
	return mHistory[mHistory.size() - 1];
}

GameState Game::gameState() const
{
	return mGameState;
}

DrawReason Game::drawReason() const
{
	if (gameState() != GAME_STATE_DRAWN)
	{
		throw IllegalStateException(
				"Cannot request draw reason of non-drawn game");
	}

	return *mDrawReason;
}

const std::vector<GameStage>& Game::gameHistory() const
{
	return mHistory;
}

Color Game::activeColor() const
{
	return currentStage().activeColor();
}

std::set<PieceMove> Game::availableMovesForPiece(const Square& square) const
{
	const GameStage& stage = currentStage();

	return details::MoveValidator::availableMovesForPiece(
			stage.board(),
			stage.move()
				? details::MoveValidator::enPassantTarget(stage.move()->pieceMove())
				: boost::optional<Square>(),
			stage.castlingRights(),
			square);
}

std::set<PieceMove> Game::allAvailableMoves() const
{
	const GameStage& stage = currentStage();

	return details::MoveValidator::allAvailableMoves(
			stage.board(),
			stage.move()
				? details::MoveValidator::enPassantTarget(stage.move()->pieceMove())
				: boost::optional<Square>(),
			stage.castlingRights(),
			stage.activeColor());
}

Game Game::makeMove(
		const PieceMove& pieceMove,
		bool offerDraw) const
{
	if (gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException("Attempted to make a move in finished game");
	}

	const Square& src = pieceMove.src();
	const std::set<PieceMove> validMoves = allAvailableMoves();

	if (validMoves.count(pieceMove) == 0)
	{
		throw IllegalStateException("Attempted to make invalid move");
	}

	const GameStage nextStage = currentStage().makeMove(pieceMove, offerDraw);

	const boost::tuple<GameState, boost::optional<DrawReason>> nextState
		= internal::inferGameStateFromStage(
				nextStage,
				mTimesPositionsReached);

	std::vector<GameStage> nextHistory = mHistory;
	nextHistory.push_back(nextStage);

	return Game(
			nextState.get<0>(),
			nextState.get<1>(),
			nextHistory);
}

boost::optional<DrawReason> Game::reasonToClaimDraw() const
{
	if (gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	// reasonToDraw() expects the map to reflect the positions reached until
	// now EXCLUDING the current position, so remove it (TODO improve by
	// passing this in the constructor)

	std::map<std::string, uint8_t> previouslyReachedPositions
		= mTimesPositionsReached;

	const std::string currentFenForReps
		= details::FenUtils::fenForRepetitions(currentStage().fen());

	previouslyReachedPositions.at(currentFenForReps)--;

	if (previouslyReachedPositions.at(currentFenForReps) == 0)
	{
		previouslyReachedPositions.erase(currentFenForReps);
	}

	return details::DrawEvaluator::reasonToDraw(
			currentStage(),
			previouslyReachedPositions);
}

Game Game::claimDraw() const
{
	if (gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	const boost::optional<DrawReason> reason = reasonToClaimDraw();

	if (!reason)
	{
		throw IllegalStateException(
				"Attempted to claim draw when it wasn't allowed");
	}

	return Game(
			GAME_STATE_DRAWN,
			reason,
			mHistory);
}

Game Game::resign(const Color resigningPlayer) const
{
	if (gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException("Cannot resign finished games");
	}

	return Game(
			(resigningPlayer == COLOR_WHITE)
				? GAME_STATE_BLACK_WON
				: GAME_STATE_WHITE_WON,
			{},
			mHistory);
}
