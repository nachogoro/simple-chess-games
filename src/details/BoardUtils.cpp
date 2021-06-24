#include "BoardUtils.h"

using namespace simplechess;

Board BoardUtils::createDefaultBoard()
{
	std::map<Square, Piece> positions;

	// Place pawns
	for (char file = 'a'; file <= 'h'; ++file)
	{
		positions.insert(
				{Square::instantiateWithRankAndFile(2, file),
				Piece(TYPE_PAWN, COLOR_WHITE, false)});

		positions.insert(
				{Square::instantiateWithRankAndFile(7, file),
				Piece(TYPE_PAWN, COLOR_BLACK, false)});
	}

	// Place rooks
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'a'),
			Piece(TYPE_ROOK, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'h'),
			Piece(TYPE_ROOK, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'a'),
			Piece(TYPE_ROOK, COLOR_BLACK, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'h'),
			Piece(TYPE_ROOK, COLOR_BLACK, false)});

	// Place knights
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'b'),
			Piece(TYPE_KNIGHT, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'g'),
			Piece(TYPE_KNIGHT, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'b'),
			Piece(TYPE_KNIGHT, COLOR_BLACK, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'g'),
			Piece(TYPE_KNIGHT, COLOR_BLACK, false)});

	// Place bishops
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'c'),
			Piece(TYPE_BISHOP, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'f'),
			Piece(TYPE_BISHOP, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'c'),
			Piece(TYPE_BISHOP, COLOR_BLACK, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'f'),
			Piece(TYPE_BISHOP, COLOR_BLACK, false)});

	// Place queens
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'd'),
			Piece(TYPE_QUEEN, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'd'),
			Piece(TYPE_QUEEN, COLOR_BLACK, false)});

	// Place kings
	positions.insert(
			{Square::instantiateWithRankAndFile(1, 'e'),
			Piece(TYPE_KING, COLOR_WHITE, false)});
	positions.insert(
			{Square::instantiateWithRankAndFile(8, 'e'),
			Piece(TYPE_KING, COLOR_BLACK, false)});

	return Board(positions);
}

Board BoardUtils::afterMove(const Board& board, const PossibleMove& move)
{
	std::map<Square, Piece> positions = board.mPiecePositions;
	if (move.moveType() == MOVE_REGULAR
			|| move.moveType() == MOVE_PAWN_PROMOTION)
	{
		const Piece piece = positions.at(move.originSquare());
		positions.insert({move.finalSquare(), piece});
		positions.erase(move.originSquare());
	}
	else
	{
		// Castling
		positions.insert({move.finalSquare(), positions.at(move.originSquare())});
		positions.erase(move.originSquare());

		const Square rookSrcSquare
			= move.originSquare().file() < move.finalSquare().file()
				? Square::instantiateWithRankAndFile(move.originSquare().rank(), 'h')
				: Square::instantiateWithRankAndFile(move.originSquare().rank(), 'a');

		const Square rookDstSquare
			= rookSrcSquare.file() == 'h'
				? Square::instantiateWithRankAndFile(rookSrcSquare.rank(), 'f')
				: Square::instantiateWithRankAndFile(rookSrcSquare.rank(), 'd');
		positions.insert({rookDstSquare, positions.at(rookSrcSquare)});
		positions.erase(rookSrcSquare);
	}

	return {positions};
}
