#include <cpp/simplechess/Piece.h>

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

bool Piece::operator==(const Piece& rhs) const
{
	return type() == rhs.type() && color() == rhs.color();
}

bool Piece::operator!=(const Piece& rhs) const
{
	return !(*this == rhs);
}

bool Piece::operator<(const Piece& rhs) const
{
	if (type() < rhs.type())
	{
		return true;
	}
	else if (type() > rhs.type())
	{
		return false;
	}

	return color() < rhs.color();
}
