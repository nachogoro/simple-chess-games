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

	Color oppositeColor(Color c);
}

#endif
