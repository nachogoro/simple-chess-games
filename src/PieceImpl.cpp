#include "PieceImpl.h"

#include <Exception.h>

using namespace simplechess;

PieceImpl::PieceImpl(
		PieceColor color,
		bool promoted,
		bool everMoved,
		std::unique_ptr<MoveBehaviour> behaviour)
	: mColor(color),
	  mPromoted(promoted),
	  mEverMoved(false),
	  mBehaviour(std::move(behaviour))
{
}

PieceImpl::PieceImpl(const PieceImpl& rhs)
	: mColor(rhs.mColor),
	  mPromoted(rhs.mPromoted),
	  mEverMoved(rhs.mEverMoved),
	  mBehaviour(rhs.mBehaviour->clone())
{
}


PieceImpl& PieceImpl::operator=(const PieceImpl& rhs)
{
	mColor = rhs.mColor;
	mPromoted = rhs.mPromoted;
	mEverMoved = rhs.mEverMoved;
	mBehaviour = std::move(rhs.mBehaviour->clone());
	return *this;
}


PieceType PieceImpl::type() const
{
	return mBehaviour->pieceType();
}


PieceColor PieceImpl::color() const
{
	return mColor;
}

void PieceImpl::onMoved()
{
	mEverMoved = true;
}

bool PieceImpl::hasEverMoved() const
{
	return mEverMoved;
}

void PieceImpl::promoteTo(std::unique_ptr<MoveBehaviour> behaviour)
{
	if (mBehaviour->pieceType() != TYPE_PAWN)
	{
		throw Exception("Cannot promote a piece which is not a pawn");
	}

	mBehaviour = std::move(behaviour);
	mPromoted = true;
}

bool PieceImpl::isPromoted() const
{
	return mPromoted;
}

Piece PieceImpl::toPublicPiece() const
{
	return Piece(type(), color(), mPromoted);
}
