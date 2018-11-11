#include "MoveBehaviourKnight.h"

using namespace simplechess;

MoveBehaviourKnight::MoveBehaviourKnight()
	: MoveBehaviour(TYPE_KNIGHT)
{
}


std::unique_ptr<MoveBehaviour> MoveBehaviourKnight::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourKnight(*this));
}


std::vector<PossibleMove> MoveBehaviourKnight::possibleMoves(
		const Square& srcSquare,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	// A knight can move in an L-shape, jumping over any piece, to any square
	// which is empty or occupied by a rival piece.

	const PieceColor color = moveHistory.size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	const PieceColor rivalColor = color == COLOR_WHITE
		? COLOR_BLACK : COLOR_WHITE;

	std::vector<PossibleMove> allMovesInEmptyBoard;

	// The knight can potentially move to eight different positions. These
	// nested for loops generate them.
	for (int step1 = 2; step1 >= -2; step1 -= 4)
	{
		for (int step2 = 1; step2 >= -1; step2 -= 2)
		{
			if (Square::isInsideBoundaries(
						srcSquare.rank() + step1, srcSquare.file() + step2))
			{
				allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
							srcSquare,
							Square::instantiateWithRankAndFile(
								srcSquare.rank() + step1,
								srcSquare.file() + step2)));
			}

			if (Square::isInsideBoundaries(
						srcSquare.rank() + step2, srcSquare.file() + step1))
			{
				allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
							srcSquare,
							Square::instantiateWithRankAndFile(
								srcSquare.rank() + step2,
								srcSquare.file() + step1)));
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

bool MoveBehaviourKnight::isValidMove(
		const Move& move,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	if (move.moveType() == MOVE_CASTLE_KING_SIDE
			|| move.moveType() == MOVE_CASTLE_QUEEN_SIDE
			|| move.moveType() == MOVE_PAWN_PROMOTION)
	{
		// A knight is never involved in castling or pawn-promotion.
		return false;
	}

	const Square& srcSquare = move.originSquare();
	const Square& dstSquare = move.finalSquare();

	if (srcSquare == dstSquare)
	{
		// A move implies changing squares
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
