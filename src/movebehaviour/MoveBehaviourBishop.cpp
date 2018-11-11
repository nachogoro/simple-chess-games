#include "MoveBehaviourBishop.h"

using namespace simplechess;

MoveBehaviourBishop::MoveBehaviourBishop()
	: MoveBehaviour(TYPE_BISHOP)
{
}


std::unique_ptr<MoveBehaviour> MoveBehaviourBishop::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourBishop(*this));
}


std::vector<PossibleMove> MoveBehaviourBishop::possibleMoves(
		const Square& srcSquare,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	// A bishop can move diagonally until it encounters another piece or the
	// end of the board
	std::vector<PossibleMove> allMovesInEmptyBoard;

	// There are four possible directions for a bishop to move. These nested
	// for loops handle all those four cases.
	for (int rankStep = 1; rankStep >= -1; rankStep -= 2)
	{
		for (int fileStep = 1; fileStep >= -1; fileStep -= 2)
		{
			uint8_t rank = srcSquare.rank() + rankStep;
			char file = srcSquare.file() + fileStep;
			while (Square::isInsideBoundaries(rank, file))
			{
				// Generate all possible moves a bishop could do from that
				// position in an empty board
				allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
							srcSquare,
							Square::instantiateWithRankAndFile(rank, file)));
				rank += rankStep;
				file += fileStep;
			}
		}
	}

	std::vector<PossibleMove> result = {};
	for (const auto& move : allMovesInEmptyBoard)
	{
		if (isValidPossibleMove(move, board, moveHistory))
		{
			result.push_back(move);
		}
	}

	return result;
}


bool MoveBehaviourBishop::isValidMove(
		const Move& move,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	if (move.moveType() == MOVE_CASTLE_KING_SIDE
			|| move.moveType() == MOVE_CASTLE_QUEEN_SIDE
			|| move.moveType() == MOVE_PAWN_PROMOTION)
	{
		// A bishop is never involved in castling or pawn-promotion.
		return false;
	}

	const Square& srcSquare = move.originSquare();
	const Square& dstSquare = move.finalSquare();

	if (srcSquare == dstSquare)
	{
		// A move implies changing squares
		return false;
	}

	if (!board.isInDiagonal(srcSquare, dstSquare))
	{
		// A bishop can only move diagonally
		return false;
	}

	if (!board.isReachable(srcSquare, dstSquare))
	{
		// A bishop cannot jump over other pieces
		return false;
	}

	const PieceColor color = moveHistory.size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	if (!board.isOccupiableBy(dstSquare, color))
	{
		// The final square is occupied by a piece of our side
		 return false;
	}

	return true;
}
