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
	 * \brief The color of a chess piece.
	 */
	enum PieceColor {
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
			 * \param promoted Whether the \c Piece is a promoted pawn.
			 */
			Piece(PieceType type, PieceColor color, bool promoted);

			/**
			 * \brief Returns the type of the \c Piece.
			 * \return The type of the \c Piece.
			 */
			PieceType type() const;

			/**
			 * \brief Returns the color of the \c Piece.
			 * \return The color of the \c Piece.
			 */
			PieceColor color() const;

			/**
			 * \brief Whether the current piece is a promoted pawn or not.
			 * \return true if it is a promoted pawn, false otherwise.
			 */
			bool promoted() const;

		private:
			PieceColor mColor;
			PieceType mType;
			bool mPromoted;
	};
}

#endif
