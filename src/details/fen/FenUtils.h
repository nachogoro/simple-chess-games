#ifndef FEN_UTILS_H_74319100_580E_47C2_8610_E38B9948AA96
#define FEN_UTILS_H_74319100_580E_47C2_8610_E38B9948AA96

#include <Piece.h>

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

			private:
				static boost::bimap<char, Piece> sPieceMap;
		};
	}
}

#endif
