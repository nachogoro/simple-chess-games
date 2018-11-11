#include "MoveBehaviourPawn.h"

using namespace simplechess;

MoveBehaviourPawn::MoveBehaviourPawn()
	: MoveBehaviour(TYPE_PAWN)
{
}


std::unique_ptr<MoveBehaviour> MoveBehaviourPawn::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourPawn(*this));
}


std::vector<PossibleMove> MoveBehaviourPawn::possibleMoves(
		const Square& srcSquare,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	// A pawn can make three possible moves:
	// - Single forward (if no piece is occupying that square)
	// - Double forward (if no piece is occupying the two squares in front of
	//   it)
	// - Capturing (either regular or "en passant")
	//
	// Pawns get promoted if they reach the end of the board.
	std::vector<PossibleMove> result = {};

	const PieceColor color = moveHistory.size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	// The last rank a pawn can be in before having to be promoted.
	const uint8_t secondToLastRank = color == COLOR_WHITE
		? 7 : 2;

	// Whether the rank increases or decreases when the pawn moves forward
	const uint8_t rankForwardStep = color == COLOR_WHITE ? 1 : -1;

	// Create all the potentially reachable squares
	std::vector<Square> reachableSquares;

	// This for loop creates the potentially three squares in the next rank
	for (uint8_t fileStep = -1; fileStep <= 1; ++fileStep)
	{
		if (Square::isInsideBoundaries(
					srcSquare.rank() + rankForwardStep,
					srcSquare.file() + fileStep))
		{
			reachableSquares.push_back(
					Square::instantiateWithRankAndFile(
						srcSquare.rank() + rankForwardStep,
						srcSquare.file() + fileStep));
		}
	}

	if (Square::isInsideBoundaries(
			srcSquare.rank() + 2*rankForwardStep, srcSquare.file()))
	{
		// Square from two ranks forward
		reachableSquares.push_back(
				Square::instantiateWithRankAndFile(
					srcSquare.rank() + 2*rankForwardStep, srcSquare.file()));
	}

	// Generate all the possible moves a pawn could make in an empty board
	std::vector<PossibleMove> allMovesInEmptyBoard;
	for (const Square& dstSquare : reachableSquares)
	{
		if (srcSquare.rank() == secondToLastRank)
		{
			allMovesInEmptyBoard.push_back(PossibleMove::pawnPromotion(
						srcSquare,
						dstSquare));
		}
		else
		{
			allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
						srcSquare,
						dstSquare));
		}
	}

	for (const auto& move : allMovesInEmptyBoard)
	{
		if (isValidPossibleMove(move, board, moveHistory))
		{
			result.push_back(move);
		}
	}

	return result;
}

bool MoveBehaviourPawn::isValidMove(
		const Move& move,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	if (move.moveType() == MOVE_CASTLE_KING_SIDE
			|| move.moveType() == MOVE_CASTLE_QUEEN_SIDE)
	{
		// A pawn is never involved in castling.
		return false;
	}

	const Square& srcSquare = move.originSquare();
	const Square& dstSquare = move.finalSquare();

	if (srcSquare.rank() == 1 || srcSquare.rank() == 8)
	{
		// A pawn can never move from the first or last rank (it should
		// have never been there)
		return false;
	}

	const PieceColor color = moveHistory.size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	const PieceColor rivalColor = color == COLOR_WHITE
		? COLOR_BLACK : COLOR_WHITE;

	// The rank in which the pawn is at the beginning of the game
	const uint8_t defaultRank = color == COLOR_WHITE
		? 2 : 7;

	// The last rank a pawn can be in before having to be promoted.
	// This coincides with the rank of the rival pawns at the beginning of the
	// game.
	const uint8_t secondToLastRank = color == COLOR_WHITE
		? 7 : 2;

	// Whether the rank increases or decreases when the pawn moves forward
	const uint8_t rankForwardStep = color == COLOR_WHITE ? 1 : -1;

	if (move.moveType() == MOVE_REGULAR
			&& srcSquare.rank() == secondToLastRank)
	{
			// If we are in the second-to-last rank, this should be a pawn
			// promotion
			return false;
	}

	if (move.moveType() == MOVE_PAWN_PROMOTION
			&& srcSquare.rank() != secondToLastRank)
	{
			// If we are not in the second-to-last rank, this should be a
			// regular move
			return false;
	}

	if ((dstSquare.rank() - srcSquare.rank())*rankForwardStep < 0)
	{
		// A pawn must always move forward
		return false;
	}

	if (std::abs(dstSquare.rank() - srcSquare.rank()) > 2)
	{
		// A pawn can move at most two ranks forward
		return false;
	}

	if (std::abs(dstSquare.rank() - srcSquare.rank()) == 2)
	{
		if (std::abs(dstSquare.file() - srcSquare.file()) != 0)
		{
			// When moving two squares forward we cannot change ranks
			return false;
		}

		const Square nextSquare = Square::instantiateWithRankAndFile(
				dstSquare.rank() + rankForwardStep,
				dstSquare.file());

		if (!board.isEmpty(nextSquare))
		{
			// There can be no obstacles when moving two squares
			return false;
		}
	}

	if (dstSquare.file() == srcSquare.file())
	{
		if (!board.isEmpty(dstSquare))
		{
			// If the pawn is moving in the same file, the final square must be
			// empty
			return false;
		}
	}
	else if (std::abs(dstSquare.file() - srcSquare.file()))
	{
		// If the pawn is changing files, it can only change to adjacent files
		 return false;
	}

	if (dstSquare.file() != srcSquare.file())
	{
		// If the pawn is moving diagonally it must be capturing (either a
		// regular capture or "en passant"), so the final square can only be
		// empty if we capturing "en passant").
		if (!board.isOccupiedByPieceOfColor(dstSquare, rivalColor))
		{
			const Square adjacentSquare = Square::instantiateWithRankAndFile(
					srcSquare.rank(), dstSquare.file());

			const Square pawnOriginSquare = Square::instantiateWithRankAndFile(
					secondToLastRank, adjacentSquare.file());

			const Move& lastMove = moveHistory.back();

			if (board.pieceAt(lastMove.finalSquare()).pieceType() != TYPE_PAWN
					|| lastMove.originSquare() != pawnOriginSquare
					|| lastMove.finalSquare() != adjacentSquare
					|| std::abs(lastMove.finalSquare().rank() - lastMove.originSquare().rank()) != 2)
			{
				// To capture "en passant" the last move must be of a rival
				// pawn to our adjacent square taking two steps
				return false;
			}
		}
	}

	return true;
}
