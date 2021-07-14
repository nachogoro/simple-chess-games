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

bool PieceMove::operator<(const PieceMove& rhs) const
{
	if (piece() < rhs.piece())
	{
		return true;
	}
	else if (piece() != rhs.piece())
	{
		return false;
	}

	if (src() < rhs.src())
	{
		return true;
	}
	else if (src() != rhs.src())
	{
		return false;
	}

	if (dst() < rhs.dst())
	{
		return true;
	}
	else if (dst() != rhs.dst())
	{
		return false;
	}

	return promoted() < rhs.promoted();
}

bool PieceMove::operator==(const PieceMove& rhs) const
{
	return piece() == rhs.piece()
		&& src() == rhs.src()
		&& dst() == rhs.dst()
		&& promoted() == rhs.promoted();
}

bool PieceMove::operator!=(const PieceMove& rhs) const
{
	return !(*this == rhs);
}
