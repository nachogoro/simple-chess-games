#ifndef GAME_STAGE_H_3064169C_7DBE_4CB3_91C2_EFE730CF43BB
#define GAME_STAGE_H_3064169C_7DBE_4CB3_91C2_EFE730CF43BB

#include <simplechess/Board.h>
#include <simplechess/Color.h>
#include <simplechess/Piece.h>
#include <simplechess/PlayedMove.h>

#include <optional>

#include <string>

namespace simplechess
{
	class GameStageBuilder;

	/**
	 * \brief The castling rights available for each side at one stage of the
	 * game.
	 */
	enum CastlingRight
	{
		/**
		 * \brief White side can castle kingside.
		 */
		WhiteKingSide  = (1 << 0),

		/**
		 * \brief White side can castle queenside.
		 */
		WhiteQueenSide = (1 << 1),

		/**
		 * \brief Black side can castle kingside.
		 */
		BlackKingSide  = (1 << 2),

		/**
		 * \brief Black side can castle queenside.
		 */
		BlackQueenSide = (1 << 3)
	};

	/**
	 * \brief A stage in the development of a game of chess.
	 *
	 * This stage is defined as the position of the pieces on the board, plus
	 * additional context from the game which cannot be inferred from it (which
	 * color plays the next move, what castling rights remain, etc.), along
	 * with the move which was last played to achieve the position.
	 */
	class GameStage
	{
		public:
			/**
			 * \brief Returns the state of the board in the current stage.
			 * \return The state of the board in the current stage.
			 */
			const Board& board() const;

			/**
			 * \brief Returns the latest move played at this stage.
			 * \return The latest move played at this stage.
			 */
			const std::optional<PlayedMove>& move() const;

			/**
			 * \brief Returns the \ref Color which is to move next.
			 *
			 * \return The \ref Color which is to move next.
			 */
			Color activeColor() const;

			/**
			 * \brief Returns a bit mask containing the \ref CastlingRight
			 * available at this stage.
			 *
			 * \note This right only means for one color that it has not moved
			 * its king nor its relevant rook, and that the rook has not been
			 * captured. It does not mean that castling is necessarily possible
			 * in the current stage.
			 *
			 * \return A bit mask containing the \ref CastlingRight
			 * available at this stage.
			 */
			uint8_t castlingRights() const;

			/** \brief Returns the number of half-moves since the last capture
			 * or pawn advance.
			 * \return The number of half-moves since the last capture or pawn
			 * advance.
			 */
			uint16_t halfMovesSinceLastCaptureOrPawnAdvance() const;

			/** \brief Returns the number of half-moves since the last capture
			 * or pawn advance.
			 * \return The number of half-moves since the last capture or pawn
			 * advance.
			 */
			uint16_t fullMoveCounter() const;

			/**
			 * \brief Returns the description of the state of the board in
			 * Forsyth-Edwards Notation.
			 * \return The description of the state of the board in FEN format.
			 */
			const std::string& fen() const;

		private:
			friend class GameStageBuilder;

			/**
			 * \brief Constructor.
			 *
			 * \param board The state of the board at this stage.
			 * \param activeColor Color whose turn it is to move.
			 * \param castlingRights A bit mask of the types of \ref
			 * CastlingRight available in the position.
			 * \param halfmoveClock Number of half-moves since the last capture
			 * or pawn advance.
			 * \param fullmoveClock The number of the full move, starting at 1
			 * and being incremented after black's move.
			 * \param move The latest move played to reach this point. Should
			 * be empty if no move has been played yet.
			 */
			GameStage(const Board& board,
					Color activeColor,
					uint8_t castlingRights,
					uint16_t halfmoveClock,
					uint16_t fullmoveClock,
					const std::string& fen,
					const std::optional<PlayedMove>& move);

		private:
			Board mBoard;
			Color mActiveColor;
			uint8_t mCastlingRights;
			uint16_t mHalfmoveClock;
			uint16_t mFullmoveClock;
			std::optional<PlayedMove> mMove;
			std::string mFen;
	};
}

#endif
