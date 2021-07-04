#include <Game.h>
#include "details/BoardAnalyzer.h"
#include "details/fen/FenParser.h"

#include <boost/tuple/tuple.hpp>

#include <algorithm>

using namespace simplechess;

namespace internal
{
	/**
	 * Returns the possible moves from a stage position.
	 */
	std::set<PieceMove> getPossibleMoves(const GameStage& stage)
	{
		// TODO idgc
	}

	bool enoughMatingMaterial(const Board& board)
	{
		// Only the following combinations are considered insufficient
		// material:
		//   - King vs King
		//   - King + Bishop vs King
		//   - King + Knight vs King
		//   - King + Bishop vs King + Bishop (same coloured bishops)
		std::set<Piece> whitePieces;
		std::set<Piece> blackPieces;

		const std::vector<Square> occupiedSquares = board.occupiedSquares();

		for (const Square& sq : occupiedSquares)
		{
			const boost::optional<Piece> piece = board.pieceAt(sq);
			if (!piece)
			{
				throw std::runtime_error(
						"Inconsistent state: no piece on a square which is "
						+ "claimed as occupied by the board");
			}

			if (piece->color() == COLOR_WHITE)
			{
				whitePieces.insert(*piece);
			}
			else
			{
				blackPieces.insert(*piece);
			}
		}

		if (whitePieces.size() == 1 || blackPieces.size() == 1)
		{
			// King vs king
			return false;
		}

		if (whitePieces.size() > 2 || blackPieces.size() > 2)
		{
			// At least one side has more than 2 pieces, this is always enough
			// to theoretically mate.
			return true;
		}

		if (whitePieces.size() == 2 && blackPieces.size() == 2)
		{
			if (!(whitePieces.count({TYPE_BISHOP, COLOR_WHITE})
					&& blackPieces.count({TYPE_BISHOP, COLOR_BLACK})))
			{
				// If both sides do not have King + Bishop but have two pieces,
				// mate is theoretically possible
				return true;
			}

			// Both sides have King + Bishop, we need to figure out if they are
			// of the same color
			boost::optional<Color> bishopColor;
			for (const Square& sq : occupiedSquares)
			{
				if (board.pieceAt(sq)->type() == TYPE_BISHOP)
				{
					if (!bishopColor)
					{
						// First bishop we encounter
						bishopColor = {sq.color()};
					}
					else
					{
						if (sq.color() != *bishopColor)
						{
							// Bishop of different colors, mate is possible
							return true;
						}
						else
						{
							// Same-colored bishops, no mate is possible
							return false;
						}
					}
				}
			}

			// Should be unreachable
			throw std::runtime_error(
					"Inconsistency when evaluating whether material "
					+ "is sufficient (bishops)");
		}

		// One side has only the King and the other has King + some other piece
		const std::set<Piece>& piecesOfRelevantSide
			= (whitePieces.size() > 1)
				? whitePieces
				: blackPieces;

		const std::set<PieceType> drawingTypes
			= {TYPE_KING, TYPE_KNIGHT, TYPE_BISHOP};

		for (const Piece& piece : piecesOfRelevantSide)
		{
			if (drawingTypes.count(piece.type()) == 0)
			{
				// Found one piece which is neither King, Bishop or Knight
				return true;
			}
		}

		return false;
	}

	bool isInCheck(const GameStage& stage)
	{
		// Easy way to verify if the current player is in check is to pretend
		// it's the other player's turn and see if it could capture the king
		const Square kingSquare = details::BoardAnalyzer::kingSquare(
				stage.board(),
				stage.activeColor());

		const GameStage invertedStage(
				stage.board,
				stage.activeColor() == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE,
				stage.castlingRights(),
				stage.halfMovesSinceLastCaptureOrPawnAdvance(),
				stage.fullMoveCounter(),
				{});

		const std::set<PieceMove> moves = getPossibleMoves(invertedStage);

		const std::set<PieceMove>::const_iterator it = std::find_if(
				moves.begin(),
				moves.end(),
				[&kingSquare] (const PieceMove& m) { return m.dst() == kingSquare; });

		return it != moves.end();
	}

	boost::tuple<GameState, boost::optional<DrawReason>> inferInitialGameState(
			const GameStage& stage)
	{
		// Since it's the initial state we discard draws due to n-fold
		// repetitions or draw offers.
		const std::set<PieceMove> possibleMoves = getPossibleMoves(stage);
		bool inCheck = isInCheck(stage);

		if (possibleMoves.size() == 0)
		{
			if (!inCheck)
			{
				return {GAME_STATE_DRAWN, {DRAW_REASON_STALEMATE}};
			}

			return {GAME_STATE_DRAWN,
				{ (stage.activeColor() == COLOR_WHITE)
					? GAME_STATE_BLACK_WON
						: GAME_STATE_WHITE_WON}};
		}

		if (stage.halfMovesSinceLastCaptureOrPawnAdvance() >= 150)
		{
			// 75-move rule
			return {GAME_STATE_DRAWN, {DRAW_REASON_75_MOVE_RULE}};
		}

		if (!enoughMatingMaterial(stage))
		{
			return {GAME_STATE_DRAWN, {DRAW_REASON_INSUFFICIENT_MATERIAL}};
		}

		return {GAME_STATE_PLAYING, {}};
	}
}

Game Game::createNewGame() {
	const std::string fenOfInitialPosition
		= "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	return createGameFromStartingFen(fenOfInitialPosition);
}

Game Game::createGameFromStartingFen(const std::string& fen)
{
	details::FenParser parsedState = details::FenParser::parse(fen);

	// We can infer the prior move if there is an en passant target
	const boost::optional<Square> epSquare = parsedState.enPassantTarget();
	boost::optional<PieceMove> lastMove;
	if (epSquare)
	{
		const Piece pawn(
				PIECE_PAWN,
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
				internal::inferInitialGameState(currentStage);

		return Game(
				fullState.get<0>(),
				fullState.get<1>(),
				currentStage);
	}

	// We can infer the last move, so we want to start the history of the game
	// one stage sooner.
	// We get the original board state by making a backwards pawn move.
	const Board originalBoardState = parsedState.board().makeMove(
			PieceMove::regularMove(
				lastMove->piece(),
				lastMove->dst(),
				lastMove->src()));

	const GameStage originalStage = {
		originalBoardState,
		(parsedState.activeColor() == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE,
		parsedState.castlingRights(),
		0, // Unknown, filler value
		parsedState.fullMoveCounter() - (parsedState.activeColor() == COLOR_WHITE ? 1 : 0),
		{} };

	const boost::tuple<GameState, boost::optional<DrawReason>> fullState =
		internal::inferInitialGameState(originalStage);

	const Game originalGame(
				fullState.get<0>(),
				fullState.get<1>(),
				originalStage);

	return originalGame.makeMove(*lastMove, false);
}
