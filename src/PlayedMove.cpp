#include "PlayedMove.h"

using namespace simplechess;

PlayedMove::PlayedMove(
		const PieceMove& pieceMove,
		const boost::optional<Piece>& capturedPiece,
		const bool drawOffered,
		const CheckType checkType,
		const std::string& algebraicNotation)
	: mPieceMove(pieceMove),
	  mCapturedPiece(capturedPiece),
	  mDrawOffered(drawOffered),
	  mCheckType(checkType),
	  mAlgebraicNotation(algebraicNotation)
{
}

const std::string& PlayedMove::inAlgebraicNotation() const
{
	return mAlgebraicNotation;
}

const PieceMove& PlayedMove::pieceMove() const
{
	return mPieceMove;
}

boost::optional<Piece>& PlayedMove::capturedPiece() const
{
	return mCapturedPiece;
}

CheckType PlayedMove::checkType() const
{
	return mCheckType;
}

bool PlayedMove::isDrawOffered() const
{
	return mDrawOffered;
}
