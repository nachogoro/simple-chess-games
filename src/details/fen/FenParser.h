#ifndef FEN_PARSER_H_7EC60DCF_6D80_4E54_BEEF_61971B784511
#define FEN_PARSER_H_7EC60DCF_6D80_4E54_BEEF_61971B784511

#include <Board.h>
#include <Color.h>
#include <GameStage.h>
#include <Piece.h>
#include <Square.h>

#include <boost/optional.hpp>

#include <stdexcept>
#include <string>

namespace simplechess
{
	namespace details
	{
		class FenParser
		{
			public:
				/**
				 * \brief Parse a FEN string and return its representation.
				 *
				 * \throws std::invalid_argument if:
				 * - \p fen is not a valid FEN string.
				 * - The situation described by the FEN string is internally
				 *   inconsistent (e.g. en passant target indicates that there
				 *   is a pawn where there isn't).
				 *
				 * \note This method does not validate chess rules (for
				 * example, whether the latest pawn move was valid, or the
				 * number of Kings on the board, or the position of pawns).
				 *
				 * \param fen The string describing the position in
				 * Forsyth-Edwards notation.
				 * \return The result of parsing the string.
				 */
				static FenParser parse(const std::string& fen);

				/**
				 * \brief Returns the state of the board described by the FEN
				 * string.
				 * \return The state of the board.
				 */
				const Board& board() const;

				/**
				 * \brief Returns the \ref Square which is a a potential target
				 * for en passant capture, or an empty value if no square is a
				 * target.
				 *
				 * If a pawn has just made a two-square move, this is the
				 * position "behind" the pawn (e.g. if a white pawn moved from
				 * e2 to e4, this would be e3).
				 *
				 * \return The \ref Square which might be a target of en
				 * passant.
				 */
				const boost::optional<Square>& enPassantTarget() const;

				/**
				 * \brief Returns the \ref Color of the active player.
				 *
				 * \return The \ref Color of the active player.
				 */
				Color activeColor() const;

				/**
				 * \brief Returns a bit mask containing the \ref CastlingRight
				 * available as described by the FEN string.
				 *
				 * \return A bit mask containing the \ref CastlingRight
				 * available as described by the FEN string.
				 */
				uint8_t castlingRights() const;

				/** \brief Returns the number of half-moves since the last
				 * capture or pawn advance.
				 * \return The number of half-moves since the last capture or
				 * pawn advance.
				 */
				uint16_t halfMovesSinceLastCaptureOrPawnAdvance() const;

				/**
				 * \brief Returns the number of the full move (1-indexed,
				 * incremented after Black's move).
				 * \return The number of the full move (1-indexed,
				 * incremented after Black's move).
				 */
				uint16_t fullMoveCounter() const;

			private:
				FenParser(
						const Board& board,
						Color activeColor,
						uint8_t castlingRights,
						const boost::optional<Square>& enPassantTarget,
						uint16_t halfmoveClock,
						uint16_t mFullmoveClock);

				Board mBoard;
				Color mActiveColor;
				uint8_t mCastlingRights;
				boost::optional<Square> mEpTarget;
				uint16_t mHalfmoveClock;
				uint16_t mFullmoveClock;
		};
	}
}

#endif
