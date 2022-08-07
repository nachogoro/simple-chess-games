#include <simplechess/Color.h>

using namespace simplechess;

Color simplechess::oppositeColor(const Color c)
{
	return c == Color::White
		? Color::Black
		: Color::White;
}
