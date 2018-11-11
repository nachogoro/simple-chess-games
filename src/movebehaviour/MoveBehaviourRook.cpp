#include "MoveBehaviourRook.h"

using namespace simplechess;

MoveBehaviourRook::MoveBehaviourRook()
	: MoveBehaviour(TYPE_ROOK)
{
}


std::unique_ptr<MoveBehaviour> MoveBehaviourRook::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourRook(*this));
}


std::vector<PossibleMove> MoveBehaviourRook::possibleMoves(
		const Square& srcSquare,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	// A rook can move horizontally or vertically until it encounters another
	// piece or the end of the board.
	std::vector<PossibleMove> allMovesInEmptyBoard;

	// There are four possible horizontal/vertical directions for a rook to
	// move. These for loops handle all those four cases.
	for (int rankStep = 1; rankStep >= -1; rankStep -= 2)
	{
		uint8_t rank = srcSquare.rank() + rankStep;
		while (Square::isInsideBoundaries(rank, srcSquare.file()))
		{
			// Generate all possible moves a rook could do from that
			// position in an empty board
			allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
						srcSquare,
						Square::instantiateWithRankAndFile(
							rank, srcSquare.file())));
			rank += rankStep;
		}
	}

	for (int fileStep = 1; fileStep >= -1; fileStep -= 2)
	{
		char file = srcSquare.file() + fileStep;
		while (Square::isInsideBoundaries(srcSquare.rank(), file))
		{
			// Generate all possible moves a rook could do from that
			// position in an empty board
			allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
						srcSquare,
						Square::instantiateWithRankAndFile(
							srcSquare.rank(), file)));
			file += fileStep;
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


bool MoveBehaviourRook::isValidMove(
		const Move& move,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	if (move.moveType() == MOVE_CASTLE_KING_SIDE
			|| move.moveType() == MOVE_CASTLE_QUEEN_SIDE
			|| move.moveType() == MOVE_PAWN_PROMOTION)
	{
		// A rook is never involved in pawn-promotion and is not the subject of
		// castling (it's the king)
		return false;
	}

	const Square& srcSquare = move.originSquare();
	const Square& dstSquare = move.finalSquare();

	if (srcSquare == dstSquare)
	{
		// A move implies changing squares
		return false;
	}

	if (!board.isInSameRankOrFile(srcSquare, dstSquare))
	{
		// A rook can only move in the same rank or file
	}

	if (!board.isReachable(srcSquare, dstSquare))
	{
		// A rook cannot jump over other pieces
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
