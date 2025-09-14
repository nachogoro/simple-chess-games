#ifndef FEN_UTILS_H_74319100_580E_47C2_8610_E38B9948AA96
#define FEN_UTILS_H_74319100_580E_47C2_8610_E38B9948AA96

#include <cpp/simplechess/GameStage.h>

#include <boost/bimap.hpp>

#include <string>

namespace simplechess
{
	namespace details
	{
		/**
		 * \brief Class of miscellaneous, auxiliary, static methods related to
		 * FEN parsing.
		 */
		class FenUtils
		{
			public:
				/**
				 * \brief Returns the string representation of \p piece.
				 * \throws std::invalid_argument if the piece cannot be
				 * represented in FEN notation.
				 * \param piece The piece whose representation is to be
				 * returned.
				 * \return The string representation of \p piece.
				 */
				static char pieceToString(const Piece& piece);

				/**
				 * \brief Returns the \ref Piece which is represented by the
				 * FEN character \p c.
				 * \throws std::invalid_argument if the character does not
				 * represent a piece in FEN notation.
				 * \param c The character to be parsed.
				 * \return The \ref Piece represented by \p c.
				 */
				static Piece stringToPiece(char c);

				/**
				 * \brief Returns a shorter version of the FEN string which can
				 * be used to check if the position has been reached before
				 * according to the criterion of the n-fold repetition rule.
				 *
				 * \param The full FEN string.
				 * \return A shorter version of the FEN string suitable to be
				 * directly compared with others to determine whether the
				 * positions are the same.
				 */
				static std::string fenForRepetitions(const std::string& fen);

				/**
				 * \brief Generate a FEN string from chess position components.
				 *
				 * \param board The board state.
				 * \param activeColor The active color.
				 * \param castlingRights The castling rights.
				 * \param epTarget The en passant target square.
				 * \param halfmoveClock The halfmove clock.
				 * \param fullmoveClock The fullmove clock.
				 * \return The FEN string representation.
				 */
				static std::string generateFen(
						const Board& board,
						const Color activeColor,
						const uint8_t castlingRights,
						const std::optional<Square>& epTarget,
						const uint16_t halfmoveClock,
						const uint16_t fullmoveClock);

				/**
				 * \brief Create a GameStage from a FEN string.
				 *
				 * \param fen The FEN string to parse.
				 * \return A GameStage representing the position described by the FEN string.
				 * \throws std::invalid_argument if the FEN string is invalid.
				 */
				static GameStage fromFenString(const std::string& fen);

			private:
				static boost::bimap<char, Piece> sPieceMap;
		};
	}
}

#endif
