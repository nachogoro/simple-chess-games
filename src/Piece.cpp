#include "Piece.h"

using namespace simplechess;

Piece::Piece(const PieceType type, const Color color)
	: mType(type),
	  mColor(color)
{
}

PieceType Piece::type() const
{
	return mType;
}

Color Piece::color() const
{
	return mColor;
}

bool Piece::operator==(const Piece& o) const
{
	return type() == o.type() && color() == o.color();
}

bool Piece::operator!=(const Piece& o) const
{
	return !(*this == o);
}

bool Piece::operator<(const Piece& o) const
{
	if (type() < o.type())
	{
		return true;
	}
	else if (type() > o.type())
	{
		return false;
	}

	return color() < o.color();
}
