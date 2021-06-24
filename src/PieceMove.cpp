#include "PieceMove.h"

using namespace simplechess;

PieceMove PieceMove::fromAlgebraicNotation(
		const Board& board,
		const std::string& algebraicNotation)
{
	// TODO
	return PieceMove(
			Piece(TYPE_PAWN, COLOR_WHITE),
			Square::instantiateFromString("e2"),
			Square::instantiateFromString("e4"),
			{});
}

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
