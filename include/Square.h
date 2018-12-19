#ifndef SQUARE_H_95959D4D_51F1_4755_A8C4_542B83FBE505
#define SQUARE_H_95959D4D_51F1_4755_A8C4_542B83FBE505

#include <cstdint>
#include <string>

namespace simplechess
{
	/**
	 * \brief A class representing a square in a chess board.
	 */
	class Square
	{
		public:
			/**
			 * \brief Factory method to instantiate a \c Square by its rank and
			 * file.
			 *
			 * Rank and file are used as per the chess algebraic notation.
			 *
			 * The method is case insensitive.
			 *
			 * \throws Exception if the rank or the file are out of their valid
			 * range.
			 *
			 * \param rank The rank of the square (a number between 1 and 8).
			 * \param file The file of the square (a letter between a and h,
			 * case insensitive).
			 * \return A \c Square object.
			 */
			static Square instantiateWithRankAndFile(uint8_t rank, char file);

			/**
			 * \brief Factory method to instantiate a \c Square from a string.
			 *
			 * The string must represent a square using the chess algebraic
			 * notation (e.g. b4).
			 *
			 * The method is case insensitive.
			 *
			 * \throws Exception if the rank or the file are out of their valid
			 * range.
			 *
			 * \param algebraicSquare A string representing a square in
			 * algebraic notation.
			 * \return A \c Square object.
			 */
			static Square instantiateFromString(
					const std::string& algebraicSquare);

			/**
			 * \brief Whether the given rank and file are inside the valid
			 * range.
			 * \param rank The rank of the square (a number between 1 and 8).
			 * \param file The file of the square (a letter between a and h).
			 * \return \c true if it is inside the boundaries of the board, \c
			 * false otherwise.
			 */
			static bool isInsideBoundaries(int rank, char file);

			/**
			 * \brief Equal comparison operator.
			 * \param rhs The \ref Square to be compared against.
			 * \return \c true if the squares are equal, \c false otherwise.
			 */
			bool operator==(const Square& rhs) const;

			/**
			 * \brief Not-equal comparison operator.
			 * \param rhs The \ref Square to be compared against.
			 * \return \c true if the squares are not equal, \c false
			 * otherwise.
			 */
			bool operator!=(const Square& rhs) const;

			/**
			 * \brief Less-than comparison operator.
			 *
			 * It does not represent any meaningful comparison, but is needed
			 * to use \ref Square as the key of a map.
			 *
			 * \param rhs The \c Square to be compared against.
			 * \return \c true if this \c Square compares to less-than \a rhs,
			 * \c false otherwise.
			 */
			bool operator<(const Square& rhs) const;

			/**
			 * \brief Returns the rank of the \c Square.
			 * \return The rank of the \c Square.
			 */
			uint8_t rank() const;

			/**
			 * \brief Returns the file of the \c Square.
			 * \return The file of the \c Square.
			 */
			char file() const;

			/**
			 * \brief Returns a string representation of the \c Square.
			 * \return A string representation of the \c Square.
			 */
			std::string toString() const;

		private:
			Square(uint8_t rank, char file);
			uint8_t mRank;
			char mFile;

			friend class AlgebraicNotationParser;
	};
}

#endif
