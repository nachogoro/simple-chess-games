#include "GameImpl.h"
#include "MoveMap.h"
#include "BoardUtils.h"
#include "BoardAnalyzer.h"
#include <Exception.h>

using namespace simplechess;

GameImpl::GameImpl(uint32_t id)
	: mId(id),
	  mBoard(BoardUtils::createDefaultBoard()),
	  mMoveHistory()
{
}

GameImpl::GameImpl(
		uint32_t id,
		const Board& board,
		const std::vector<Move>& moveHistory,
		const std::vector<Piece>& capturedPieces)
	: mId(id),
	  mBoard(board),
	  mMoveHistory(moveHistory),
	  mCapturedPieces(capturedPieces)
{
}

GameImpl GameImpl::parseFromFile(
		const std::string& /*file*/, uint32_t id)
{
	// TODO
	return GameImpl(id);
}

Game GameImpl::toPublicGame() const
{
	return Game(mId, mBoard, mMoveHistory.getAllMoves(), mCapturedPieces);
}

void GameImpl::exportToFile(const std::string& outFile) const
{
	// TODO
}

std::vector<PossibleMove> GameImpl::possibleMoves(
		const Square& srcSquare) const
{
	const Color toPlay
		= mMoveHistory.getAllMoves().size() % 2 == 0
			? COLOR_WHITE
			: COLOR_BLACK;

	const Color rivalColor = toPlay
		== COLOR_WHITE
			? COLOR_BLACK
			: COLOR_WHITE;

	const boost::optional<Piece>& pieceToMove = mBoard.pieceAt(srcSquare);

	if (!pieceToMove || pieceToMove->color() != toPlay)
	{
		throw Exception("Player cannot move piece at " + srcSquare.toString());
	}

	std::vector<PossibleMove> moves
		= MoveMap::getBehaviourForPiece(pieceToMove->type()).possibleMoves(
				srcSquare, mBoard, mMoveHistory);

	// moves might contain actually invalid moves, because it does not check
	// that the king is not left in check
	std::vector<PossibleMove> result;

	for (const auto& move : moves)
	{
		Board boardAfterMove = BoardUtils::afterMove(mBoard, move);
		if (!BoardAnalyzer::isSquareThreatenedBy(
					mBoard,
					BoardAnalyzer::kingSquare(mBoard, toPlay),
					rivalColor,
					mMoveHistory))
		{
			result.push_back(move);
		}
	}

	return result;
}

Game GameImpl::makeMove(const Move& move)
{
	// TODO
	throw Exception("");
}
