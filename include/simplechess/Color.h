#ifndef COLOR_H_DF81538F_CDF8_4535_A288_BB24FBEFD656
#define COLOR_H_DF81538F_CDF8_4535_A288_BB24FBEFD656

namespace simplechess
{
	/**
	 * \brief The color of each side in a chess game.
	 */
	enum class Color
	{
		White,
		Black
	};

	/**
	 * \brief Returns the color of the other side.
	 *
	 * \param c The \ref Color whose opposite is wanted.
	 * \return \ref Color::Black for \ref Color::White and vice versa.
	 */
	Color oppositeColor(Color c);
}

#endif
