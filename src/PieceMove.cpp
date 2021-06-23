#include "PieceMove.h"

using namespace simplechess;

PieceMove PieceMove::regularMove(
		const Piece& piece,
		const Square& src,
		const Square& dst)
{
	return PieceMove(
			piece,
			src,
			dst,
			{});
}

PieceMove PieceMove::pawnPromotion(
		const Piece& piece,
		const Square& src,
		const Square& dst,
		const PieceType promotedType)
{
	return PieceMove(
			piece,
			src,
			dst,
			{promotedType});
}

PieceMove::PieceMove(
		const Piece& piece,
		const Square& src,
		const Square& dst,
		const boost::optional<PieceType>& promotedType)
	: mPiece(piece),
	  mSrc(src),
	  mDst(dst),
	  mPromotedType(promotedType)
{
}

const Piece& PieceMove::piece() const
{
	return mPiece;
}

const Square& PieceMove::src() const
{
	return mSrc;
}

const Square& PieceMove::dst() const
{
	return mDst;
}

const boost::optional<PieceType>& PieceMove::promoted() const
{
	return mPromotedType;
}

bool PieceMove::operator<(const PieceMove& o) const
{
	if (piece() < o.piece())
	{
		return true;
	}
	else if (piece() != o.piece())
	{
		return false;
	}

	if (src() < o.src())
	{
		return true;
	}
	else if (src() != o.src())
	{
		return false;
	}

	if (dst() < o.dst())
	{
		return true;
	}
	else if (dst() != o.dst())
	{
		return false;
	}

	return promoted() < o.promoted();
}

bool PieceMove::operator==(const PieceMove& o) const
{
	return piece() == o.piece()
		&& src() == o.src()
		&& dst() == o.dst()
		&& promoted() == o.promoted();
}

bool PieceMove::operator!=(const PieceMove& o) const
{
	return !(*this == o);
}
