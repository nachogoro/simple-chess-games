#include "GameManagerImpl.h"

#include "details/GameStageUpdater.h"
#include "details/fen/FenParser.h"

using namespace simplechess;

Game GameManagerImpl::createNewGame() const
{
	const std::string fenOfInitialPosition
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	return createGameFromFen(fenOfInitialPosition);
}

Game GameManagerImpl::createGameFromFen(const std::string& fen) const
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

		const Square src = Square::fromRankAndFile(
				((pawn.color() == COLOR_WHITE)
					? 2
					: 7),
				epSquare->file());

		const Square dst = Square::fromRankAndFile(
				((pawn.color() == COLOR_WHITE)
					? 4
					: 5),
				epSquare->file());

		lastMove = { PieceMove::regularMove(pawn, src, dst) };
	}

	// TODO validate here everything related to move counters, number of
	// kings, consistency of the en passant field, etc

	if (!lastMove)
	{
		const GameStage currentStage = {
			parsedState.board(),
			parsedState.activeColor(),
			parsedState.castlingRights(),
			parsedState.halfMovesSinceLastCaptureOrPawnAdvance(),
			parsedState.fullMoveCounter(),
			fen,
			{}};

		const GameStateInformation information = GameStateDetector::detect(
				currentStage,
				{});

		return Game(
				information.gameState,
				information.reasonItWasDrawn,
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

	const GameStateInformation information = GameStateDetector::detect(
			originalStage,
			{});

	const Game originalGame(
			information.gameState,
			information.reasonItWasDrawn,
			{originalStage},
			information.availableMoves,
			information.reasonToClaimDraw);

	return makeMove(originalGame, *lastMove, false);
}

Game GameManagerImpl::makeMove(
		const Game& game,
		const PieceMove& move,
		bool offerDraw=false) const
{
	if (game.gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException("Attempted to make a move in finished game");
	}

	const Square& src = pieceMove.src();
	const std::set<PieceMove> validMoves = game.allAvailableMoves();

	if (validMoves.count(pieceMove) == 0)
	{
		throw IllegalStateException("Attempted to make invalid move");
	}

	const GameStage nextStage = details::GameStageUpdater::makeMove(
			game.currentStage(),
			pieceMove,
			offerDraw);

	const GameStateInformation information = GameStateDetector::detect(
			nextStage,
			internal::getPreviouslyReachedPositionsMap(game.history()));

	std::vector<GameStage> nextHistory = mHistory;
	nextHistory.push_back(nextStage);

	return Game(
			information.gameState,
			information.reasonItWasDrawn,
			nextHistory,
			information.availableMoves,
			information.reasonToClaimDraw);
}

Game GameManagerImpl::claimDraw(const Game& game) const
{
	if (game.gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException(
				"Draws cannot be claimed in finished games");
	}

	const boost::optional<DrawReason> reason = game.reasonToClaimDraw();

	if (!reason)
	{
		throw IllegalStateException(
				"Attempted to claim draw when it wasn't allowed");
	}

	return Game(
			GAME_STATE_DRAWN,
			reason,
			game.history(),
			{},
			{});
}

Game GameManagerImpl::resign(const Game& game, Color resigningPlayer) const
{
	if (game.gameState() != GAME_STATE_PLAYING)
	{
		throw IllegalStateException("Cannot resign finished games");
	}

	return Game(
			(resigningPlayer == COLOR_WHITE)
				? GAME_STATE_BLACK_WON
				: GAME_STATE_WHITE_WON,
			{},
			game.history(),
			{},
			{});
}
