#include <Square.h>
#include <Exception.h>
#include <cctype>

using namespace simplechess;

Square Square::instantiateWithRankAndFile(uint8_t rank, char file)
{
	if (!isInsideBoundaries(rank, file))
	{
		throw Exception("Square is outside the boundaries: "
				+ std::to_string(rank) + std::to_string(file));
	}

	return Square(rank, tolower(file));
}

Square Square::instantiateFromString(const std::string& algebraicSquare)
{
	// TODO
	return Square(1, 'a');
}

bool Square::isInsideBoundaries(int rank, char file)
{
	file = tolower(file);
	return (rank >= 1 && rank <= 8 && file >= 'a' && file <= 'h');
}

bool Square::operator==(const Square& rhs) const
{
	return rhs.rank() == rank() && rhs.file() == file();
}

bool Square::operator!=(const Square& rhs) const
{
	return !(rhs == *this);
}

bool Square::operator<(const Square& rhs) const
{
	if (rank() < rhs.rank())
		return true;

	if (file() < rhs.file())
		return true;

	return false;
}

uint8_t Square::rank() const
{
	return mRank;
}

char Square::file() const
{
	return mFile;
}

std::string Square::toString() const
{
	// TODO
	return "1a";
}
