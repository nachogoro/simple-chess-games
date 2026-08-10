#ifndef BUILDERS_H_E5875D9E_C927_464E_BC23_2AA288A34B41
#define BUILDERS_H_E5875D9E_C927_464E_BC23_2AA288A34B41

#include <simplechess/Game.h>
#include <simplechess/GameStage.h>

/**
 * This file contains all the builders to create objects from the public
 * interface which are not supposed to be created by the outside user of the
 * library.
 *
 * The purpose of this scheme is to keep the constructors private in those
 * objects while at the same time not having to list every single internal
 * class which might instantiate them in the public header files.
 */

namespace simplechess
{
	class GameStageBuilder
	{
		public:
			static GameStage build(
				const Board& board,
				Color toPlay,
				uint8_t castlingRights,
				uint16_t halfmoveClock,
				uint16_t fullmoveClock,
				const std::optional<Square>& enPassantTarget);

			/**
			 * Overload for callers which have already determined the check
			 * status of the position, so that it is not derived twice.
			 */
			static GameStage build(
				const Board& board,
				Color toPlay,
				uint8_t castlingRights,
				uint16_t halfmoveClock,
				uint16_t fullmoveClock,
				const std::optional<Square>& enPassantTarget,
				CheckType checkStatus);

			/**
			 * \brief Rebuilds a stage from a FEN string which is already
			 * known to describe it, together with its check status.
			 *
			 * Nothing has to be derived: the FEN is kept as given instead of
			 * being regenerated from the position it was just parsed out of,
			 * and the check status is taken on trust rather than worked out
			 * again. Only use this where both are known to be consistent
			 * with the position - typically when restoring a stage which
			 * this library produced earlier.
			 */
			static GameStage buildFromKnownFen(
				const std::string& fen,
				CheckType checkStatus);
	};

	class GameBuilder
	{
		public:
			/**
			 * The by-value parameters are moved into the resulting \ref Game,
			 * so callers should move into them: the history in particular is
			 * expensive to copy and grows with every move played.
			 */
			static Game build(
					GameState gameState,
					const std::optional<DrawReason>& drawReason,
					std::vector<HistoryEntry> history,
					GameStage currentStage,
					std::vector<PieceMove> allAvailableMoves,
					const std::optional<DrawReason>& reasonToClaimDraw,
					DrawEnforcement drawEnforcement = DrawEnforcement::Automatic,
					std::map<std::string, uint8_t> previouslyReachedPositions = {});

			/**
			 * Grants the library's internals access to the repetition
			 * bookkeeping cached inside a \ref Game, which is deliberately
			 * not part of its public interface.
			 */
			static const std::map<std::string, uint8_t>&
				previouslyReachedPositions(const Game& game);
	};

	class BoardBuilder
	{
		public:
			static Board build(
					const std::map<Square, Piece> positions);
	};

	class PlayedMoveBuilder
	{
		public:
			static PlayedMove build(
					const Board& board,
					const PieceMove& move,
					const bool drawOffered);

			/**
			 * Overload for callers which already know the check status the
			 * move produces (it is the check status of the stage the move
			 * leads to), sparing a second analysis of the resulting position.
			 */
			static PlayedMove build(
					const Board& board,
					const PieceMove& move,
					const bool drawOffered,
					CheckType checkType);

			static PlayedMove build(
					const PieceMove& pieceMove,
					const std::optional<Piece>& capturedPiece,
					bool drawOffered,
					CheckType checkType,
					const std::string& algebraicNotation);
	};
}

#endif
