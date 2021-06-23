#include <Square.h>

#include <stdexcept>
#include <sstream>
#include <cctype>

using namespace simplechess;

Square Square::instantiateWithRankAndFile(uint8_t rank, char file)
{
	if (!isInsideBoundaries(rank, file))
	{
		std::stringstream ss;
		ss << "Square is outside the boundaries: " << static_cast<int>(rank) << file;
		throw std::invalid_argument(ss.str());
	}

	return Square(rank, tolower(file));
}

Square Square::instantiateFromString(const std::string& algebraicSquare)
{
	if (algebraicSquare.size() != 2
			|| !std::isalpha(algebraicSquare[0])
			|| !std::isdigit(algebraicSquare[1]))
	{
		throw std::invalid_argument(algebraicSquare + " is not a valid square");
	}

	return instantiateWithRankAndFile(
			static_cast<uint8_t>(algebraicSquare[1] - '0'),
			algebraicSquare[0]);
}

bool Square::isInsideBoundaries(uint8_t rank, char file)
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
	if (rank() > rhs.rank())
	{
		return true;
	} else if (rank() < rhs.rank()) {
		return false;
	}

	return file() < rhs.file();
}

uint8_t Square::rank() const
{
	return mRank;
}

char Square::file() const
{
	return mFile;
}

Color Square::color() const
{
	// If we 1-index the files, all the squares in which both file and rank are
	// even or odd are black
	uint8_t fileIndex = file() - 'a' + 1;

	return ((fileIndex % 2) == (rank() % 2))
		? COLOR_BLACK
		: COLOR_WHITE;
}

std::string Square::toString() const
{
	std::stringstream ss;
	ss << file() << static_cast<int>(rank());
	return ss.str();
}

Square::Square(const uint8_t rank, const char file)
	: mRank(rank),
	  mFile(file)
{
}
