#include "TestUtils.h"

#include <gtest/gtest.h>

#include <vector>

using namespace simplechess;

namespace
{
	const std::vector<PieceMove> OPENING = {
		PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				Square::fromString("e2"),
				Square::fromString("e4")),
		PieceMove::regularMove(
				{PieceType::Pawn, Color::Black},
				Square::fromString("e7"),
				Square::fromString("e5")),
		PieceMove::regularMove(
				{PieceType::Knight, Color::White},
				Square::fromString("g1"),
				Square::fromString("f3")),
	};
}

TEST(HistoryTest, EntryHoldsThePositionItsMoveWasPlayedFrom) {
	// Every position is kept as the game reaches it, so that the history is
	// compared against the positions themselves rather than against FEN
	// strings written out here.
	std::vector<Game> asReached = {createNewGame()};

	for (const PieceMove& move : OPENING)
	{
		asReached.push_back(makeMove(asReached.back(), move));
		EXPECT_EQ(asReached.back().history().size(), asReached.size() - 1);
	}

	const Game& played = asReached.back();
	ASSERT_EQ(played.history().size(), OPENING.size());

	for (size_t i = 0; i < OPENING.size(); ++i)
	{
		const HistoryEntry& entry = played.history()[i];

		EXPECT_EQ(entry.stage.fen(), asReached[i].currentStage().fen())
			<< "at index " << i;
		EXPECT_EQ(entry.move.pieceMove(), OPENING[i])
			<< "at index " << i;
	}

	// The position of the last entry is the one the last move was played
	// from, so it is not the position the game now stands in.
	EXPECT_NE(played.history().back().stage.fen(),
			played.currentStage().fen());
}

TEST(HistoryTest, EntriesAreOrderedFromTheOpeningPosition) {
	const Game game = makeMove(
			makeMove(createNewGame(), OPENING[0]),
			OPENING[1]);

	ASSERT_EQ(game.history().size(), 2);

	// The first entry is the game as it was created, whatever has been
	// played since.
	EXPECT_EQ(game.history().front().stage.fen(),
			createNewGame().currentStage().fen());

	EXPECT_EQ(game.history()[0].stage.activeColor(), Color::White);
	EXPECT_EQ(game.history()[1].stage.activeColor(), Color::Black);
	EXPECT_EQ(game.currentStage().activeColor(), Color::White);
}
