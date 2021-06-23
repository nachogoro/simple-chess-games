#ifndef PIECE_H_F9AD4EA8_033F_4911_BDF4_6FCAA1DA336D
#define PIECE_H_F9AD4EA8_033F_4911_BDF4_6FCAA1DA336D

namespace simplechess
{
	/**
	 * \brief The type of a chess piece.
	 */
	enum PieceType{
		TYPE_PAWN,
		TYPE_ROOK,
		TYPE_KNIGHT,
		TYPE_BISHOP,
		TYPE_QUEEN,
		TYPE_KING
	};

	/**
	 * \brief The color of each side in a chess game.
	 */
	enum Color {
		COLOR_WHITE,
		COLOR_BLACK
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

		private:
			Color mColor;
			PieceType mType;
	};
}

#endif
