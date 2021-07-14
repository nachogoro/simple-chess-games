#ifndef PIECE_H_F9AD4EA8_033F_4911_BDF4_6FCAA1DA336D
#define PIECE_H_F9AD4EA8_033F_4911_BDF4_6FCAA1DA336D

#include <Color.h>

namespace simplechess
{
	/**
	 * \brief The type of a chess piece.
	 */
	enum PieceType{
		/**
		 * \brief Pawn.
		 */
		TYPE_PAWN,

		/**
		 * \brief Rook.
		 */
		TYPE_ROOK,

		/**
		 * \brief Knight.
		 */
		TYPE_KNIGHT,

		/**
		 * \brief Bishop.
		 */
		TYPE_BISHOP,

		/**
		 * \brief Queen.
		 */
		TYPE_QUEEN,

		/**
		 * \brief King.
		 */
		TYPE_KING
	};

	/**
	 * \brief A chess piece.
	 */
	class Piece
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * Instantiates a \c Piece of the given \c type and \c color.
			 *
			 * \param type The type of the \c Piece.
			 * \param color The color of the \c Piece.
			 */
			Piece(PieceType type, Color color);

			/**
			 * \brief Returns the type of the \c Piece.
			 * \return The type of the \c Piece.
			 */
			PieceType type() const;

			/**
			 * \brief Returns the color of the \c Piece.
			 * \return The color of the \c Piece.
			 */
			Color color() const;

			/**
			 * \brief Less-than comparison operator.
			 *
			 * The order established by  this operator is meaningless, as this
			 * method is only necessary to allow objects of this class to be
			 * used in std::set collections or as keys in std::map.
			 *
			 * \param rhs The \c Piece to be compared against.
			 * \return \c true if this \c Piece is considered lees than \a rhs,
			 * \c false otherwise.
			 */
			bool operator<(const Piece& rhs) const;

			/**
			 * \brief Equals comparison operator.
			 * \param rhs The \c Piece to be compared against.
			 * \return \c true if this \c Piece is identical to \a rhs, \c
			 * false otherwise.
			 */
			bool operator==(const Piece& rhs) const;

			/**
			 * \brief Not-equals comparison operator.
			 * \param rhs The \c Piece to be compared against.
			 * \return \c true if this \c Piece is not identical to \a rhs, \c
			 * false otherwise.
			 */
			bool operator!=(const Piece& rhs) const;


		private:
			PieceType mType;
			Color mColor;
	};
}

#endif
