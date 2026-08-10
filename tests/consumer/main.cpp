#include <simplechess/SimpleChess.h>

#include <cstdio>
#include <optional>

using namespace simplechess;

int main()
{
	const Game game = createNewGame();

	const std::optional<PieceMove> move = findMove(
			game, Square::fromString("g1"), Square::fromString("f3"));

	if (!move)
	{
		std::puts("no such move");
		return 1;
	}

	const Game played = makeMove(game, *move);

	if (played.history().back().move.inAlgebraicNotation() != std::string("Nf3"))
	{
		std::puts("unexpected move");
		return 1;
	}

	std::printf("C++ consumer: %s, %zu moves available\n",
			played.history().back().move.inAlgebraicNotation().c_str(),
			played.allAvailableMoves().size());
	return 0;
}
