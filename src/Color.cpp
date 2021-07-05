#include "Color.h"

using namespace simplechess;

Color oppositeColor(const Color c)
{
	return c == COLOR_WHITE
		? COLOR_BLACK
		: COLOR_WHITE;
}