#include "MoveMap.h"
#include "movebehaviour/MoveBehaviourPawn.h"
#include "movebehaviour/MoveBehaviourRook.h"
#include "movebehaviour/MoveBehaviourKnight.h"
#include "movebehaviour/MoveBehaviourBishop.h"
#include "movebehaviour/MoveBehaviourQueen.h"
#include "movebehaviour/MoveBehaviourKing.h"

using namespace simplechess;

namespace
{
	std::map<PieceType, std::unique_ptr<MoveBehaviour>> initializeMap()
	{
		std::map<PieceType, std::unique_ptr<MoveBehaviour>> result;
		result.insert(std::make_pair(TYPE_PAWN, std::unique_ptr<MoveBehaviour>(new MoveBehaviourPawn())));
		result.insert(std::make_pair(TYPE_ROOK, std::unique_ptr<MoveBehaviour>(new MoveBehaviourRook())));
		result.insert(std::make_pair(TYPE_KNIGHT, std::unique_ptr<MoveBehaviour>(new MoveBehaviourKnight())));
		result.insert(std::make_pair(TYPE_BISHOP, std::unique_ptr<MoveBehaviour>(new MoveBehaviourBishop())));
		result.insert(std::make_pair(TYPE_QUEEN, std::unique_ptr<MoveBehaviour>(new MoveBehaviourQueen())));
		result.insert(std::make_pair(TYPE_KING, std::unique_ptr<MoveBehaviour>(new MoveBehaviourKing())));
		return result;
	}
}

std::map<PieceType, std::unique_ptr<MoveBehaviour>>
MoveMap::mPieceToBehaviour = initializeMap();

const MoveBehaviour& MoveMap::getBehaviourForPiece(PieceType piece)
{
	return *mPieceToBehaviour.at(piece);
}
