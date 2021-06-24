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
