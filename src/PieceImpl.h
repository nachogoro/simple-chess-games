#ifndef PIECEIMPL_H_D9BA02C4_9CCB_4C94_A75C_54BCC893D196
#define PIECEIMPL_H_D9BA02C4_9CCB_4C94_A75C_54BCC893D196

#include <Piece.h>
#include <Square.h>
#include <Move.h>
#include <PossibleMove.h>

#include "BoardImpl.h"
#include "movebehaviour/MoveBehaviour.h"

#include <memory>
#include <vector>

namespace simplechess
{
	class PieceImpl
	{
		public:
			static PieceImpl createPawn(PieceColor color);
			static PieceImpl createRook(PieceColor color);
			static PieceImpl createKnight(PieceColor color);
			static PieceImpl createBishop(PieceColor color);
			static PieceImpl createQueen(PieceColor color);
			static PieceImpl createKing(PieceColor color);

			// Copy constructor
			PieceImpl(const PieceImpl& rhs);

			// Assignment operator
			PieceImpl& operator=(const PieceImpl& rhs);

			PieceType type() const;
			PieceColor color() const;

			// Invoked when the piece is moved
			void onMoved();

			bool hasEverMoved() const;

			void promoteTo(std::unique_ptr<MoveBehaviour> behaviour);

			bool isPromoted() const;

			Piece toPublicPiece() const;

		private:
			PieceImpl(
					PieceColor color,
					bool promoted,
					bool everMoved,
					std::unique_ptr<MoveBehaviour> behaviour);

		private:
			PieceColor mColor;
			bool mPromoted;
			bool mEverMoved;
			std::unique_ptr<MoveBehaviour> mBehaviour;
	};
}

#endif
