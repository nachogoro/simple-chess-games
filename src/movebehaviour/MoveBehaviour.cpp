#include "MoveBehaviour.h"
#include "Exception.h"

using namespace simplechess;

MoveBehaviour::MoveBehaviour(PieceType type)
	: mPieceType(type)
{
}

PieceType MoveBehaviour::pieceType() const
{
	return mPieceType;
}

bool MoveBehaviour::isValidPossibleMove(
		const PossibleMove& possibleMove,
		const BoardImpl& board,
		const std::vector<Move>& moveHistory) const
{
	switch (possibleMove.moveType())
	{
		case MOVE_REGULAR:
		{
			const Move move = Move::regularMove(
					possibleMove.originSquare(),
					possibleMove.finalSquare());
			return isValidMove(move, board, moveHistory);
		}
		case MOVE_PAWN_PROMOTION:
		{
			const Move move = Move::pawnPromotion(
					possibleMove.originSquare(),
					possibleMove.finalSquare(),
					TYPE_QUEEN);
			return isValidMove(move, board, moveHistory);
		}
		case MOVE_CASTLE_KING_SIDE:
		{
			const Move move = Move::castle(
					possibleMove.originSquare(),
					CASTLING_KING_SIDE);
			return isValidMove(move, board, moveHistory);
		}
		case MOVE_CASTLE_QUEEN_SIDE:
		{
			const Move move = Move::castle(
					possibleMove.originSquare(),
					CASTLING_QUEEN_SIDE);
			return isValidMove(move, board, moveHistory);
		}
		default:
		{
			throw Exception("Unknown move type: "
					+ std::to_string(possibleMove.moveType()));
		}
	}
}
