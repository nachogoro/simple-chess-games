#include <cpp/simplechess/Square.h>

#include <stdexcept>
#include <sstream>
#include <cctype>

using namespace simplechess;

Square Square::fromRankAndFile(uint8_t rank, char file)
{
	if (!isInsideBoundaries(rank, file))
	{
		std::stringstream ss;
		ss << "Square is outside the boundaries: " << static_cast<int>(rank) << file;
		throw std::invalid_argument(ss.str());
	}

	return Square(rank, asciiToLower(file));
}

Square Square::fromString(const std::string& algebraicSquare)
{
	if (algebraicSquare.size() != 2
			|| !std::isalpha(algebraicSquare[0])
			|| !std::isdigit(algebraicSquare[1]))
	{
		throw std::invalid_argument(algebraicSquare + " is not a valid square");
	}

	return fromRankAndFile(
			static_cast<uint8_t>(algebraicSquare[1] - '0'),
			algebraicSquare[0]);
}

Color Square::color() const
{
	// If we 1-index the files, all the squares in which both file and rank are
	// even or odd are black
	uint8_t fileIndex = file() - 'a' + 1;

	return ((fileIndex % 2) == (rank() % 2))
		? Color::Black
		: Color::White;
}

std::string Square::toString() const
{
	// Always exactly two characters, so building the string directly avoids
	// standing up a whole stream for it.
	return std::string{file(), static_cast<char>('0' + rank())};
}

Square::Square(const uint8_t rank, const char file)
	: mRank(rank),
	  mFile(file)
{
}
