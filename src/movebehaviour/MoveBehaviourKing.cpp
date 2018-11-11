#include "MoveBehaviourKing.h"

using namespace simplechess;

MoveBehaviourKing::MoveBehaviourKing()
	: MoveBehaviour(TYPE_KING)
{
}


std::unique_ptr<MoveBehaviour> MoveBehaviourKing::clone() const
{
	return std::unique_ptr<MoveBehaviour>(new MoveBehaviourKing(*this));
}


std::vector<PossibleMove> MoveBehaviourKing::possibleMoves(
		const Square& srcSquare,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	// A king can move one square in any direction.
	// It can also castle with a rook if:
	// 	- There are no pieces between the king and the rook.
	// 	- The king would not be in check while moving to its new position.
	// 	- Neither the king nor the rook have been moved before.
	std::vector<PossibleMove> allMovesInEmptyBoard;

	//There are eight possible squares for the king to move without castling.
	//These nested for loops handle all those cases.
	for (int rankStep = 1; rankStep >= -1; --rankStep)
	{
		for (int fileStep = 1; fileStep >= -1; --fileStep)
		{
			if (rankStep == 0 && fileStep == 0)
			{
				continue;
			}

			if (Square::isInsideBoundaries(
						srcSquare.rank() + rankStep,
						srcSquare.file() + fileStep))
			{
				allMovesInEmptyBoard.push_back(PossibleMove::regularMove(
							srcSquare,
							Square::instantiateWithRankAndFile(
								srcSquare.rank() + rankStep,
								srcSquare.file() + fileStep)));
			}
		}
	}

	const PieceColor color = moveHistory.size() % 2 == 0
		? COLOR_WHITE : COLOR_BLACK;

	// Add castling
	if (!board.pieceAt(srcSquare).hasEverMoved())
	{
		allMovesInEmptyBoard.push_back(
				PossibleMove::castle(srcSquare, CASTLING_QUEEN_SIDE));
		allMovesInEmptyBoard.push_back(
				PossibleMove::castle(srcSquare, CASTLING_KING_SIDE));
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


bool MoveBehaviourKing::isValidMove(
		const Move& move,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	if (move.moveType() == MOVE_PAWN_PROMOTION)
	{
		// A king is never involved in pawn-promotion.
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

	const PieceColor rivalColor = color == COLOR_WHITE
		? COLOR_BLACK : COLOR_WHITE;


	if (move.moveType() == MOVE_REGULAR)
	{
		if (board.isThreatenedBy(dstSquare, rivalColor)
				|| std::abs(srcSquare.rank() - dstSquare.rank()) > 1
				|| std::abs(srcSquare.file() - dstSquare.file()) > 1)
		{
			// The king cannot move more than one square and cannot move to a
			// position of threat
			return false;
		}
		else
		{
			return true;
		}
	}

	// At this point we are dealing with castling
	if (board.pieceAt(srcSquare).hasEverMoved())
	{
		// The king cannot have moved for castling
		return false;
	}

	Square rookSquare = Square::instantiateFromString("a1");
	if (color == COLOR_WHITE)
	{
		if (move.moveType() == MOVE_CASTLE_KING_SIDE)
		{
			rookSquare = Square::instantiateFromString("h1");
		}
		else
		{
			rookSquare = Square::instantiateFromString("a1");
		}
	}
	else
	{
		if (move.moveType() == MOVE_CASTLE_KING_SIDE)
		{
			rookSquare = Square::instantiateFromString("h8");
		}
		else
		{
			rookSquare = Square::instantiateFromString("a8");
		}
	}

	if (board.isEmpty(rookSquare)
			|| board.pieceAt(rookSquare).hasEverMoved()
			|| board.pieceAt(rookSquare).pieceType() != TYPE_ROOK
			|| board.pieceAt(rookSquare).pieceColor() != color)
	{
		// The rook cannot have moved from its original position
		return false;
	}

	if (!board.isReachable(srcSquare, rookSquare))
	{
		// There can be no pieces between the rook and the king for castling
		return false;
	}

	// The king cannot be checked while moving to its new position
	const int fileStep = dstSquare.file() - srcSquare.file() > 0 ? 1 : -1;
	char file = srcSquare.file() + fileStep;
	while(file != dstSquare.file())
	{
		if (board.isThreatenedBy(
					Square::instantiateWithRankAndFile(srcSquare.rank(), file),
					rivalColor))
		{
			return false;
		}
	}

	return true;
}
