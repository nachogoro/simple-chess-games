#include "MoveBehaviourQueen.h"
#include "BoardAnalyzer.h"

using namespace simplechess;

std::unique_ptr<MoveBehaviour> MoveBehaviourQueen::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourQueen(*this));
}


std::vector<PossibleMove> MoveBehaviourQueen::possibleMoves(
		const Square& srcSquare,
		const Board& board,
		const MoveHistory& moveHistory) const
{
	// A queen can move horizontally, vertically or diagonally until it
	// encounters another piece or the end of the board
	std::vector<PossibleMove> allMovesInEmptyBoard;

	// There are four possible diagonal directions for a queen to move. These
	// nested for loops handle all those four cases.
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

	// There are four possible horizontal/vertical directions for a queen to
	// move. These for loops handle all those four cases.
	for (int rankStep = 1; rankStep >= -1; rankStep -= 2)
	{
		uint8_t rank = srcSquare.rank() + rankStep;
		while (Square::isInsideBoundaries(rank, srcSquare.file()))
		{
			// Generate all possible moves a queen could do from that
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
			// Generate all possible moves a queen could do from that
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


bool MoveBehaviourQueen::isValidMove(
		const Move& move,
		const Board& board,
		const MoveHistory& moveHistory) const
{
	if (move.moveType() == MOVE_CASTLE_KING_SIDE
			|| move.moveType() == MOVE_CASTLE_QUEEN_SIDE
			|| move.moveType() == MOVE_PAWN_PROMOTION)
	{
		// A queen is never involved in castling or pawn-promotion.
		return false;
	}

	const Square& srcSquare = move.originSquare();
	const Square& dstSquare = move.finalSquare();

	if (srcSquare == dstSquare)
	{
		// A move implies changing squares
		return false;
	}

	if (!BoardAnalyzer::isReachable(board, srcSquare, dstSquare))
	{
		// A queen cannot jump over other pieces
		return false;
	}

	const Color color = moveHistory.getAllMoves().size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	if (!BoardAnalyzer::isOccupiableBy(board, dstSquare, color))
	{
		// The final square is occupied by a piece of our side
		 return false;
	}

	return true;
}
