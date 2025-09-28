#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(GameCreationTest, RegularGameCreation) {
	const Game game = createNewGame();

	EXPECT_EQ(game.gameState(), GameState::Playing);

	EXPECT_THROW_CUSTOM(game.drawReason(), IllegalStateException);

	const auto& history = game.history();

	EXPECT_EQ(history.size(), 0);
	EXPECT_EQ(game.activeColor(), Color::White);
	EXPECT_EQ(game.reasonToClaimDraw(), std::optional<DrawReason>());

	// Validate piece positions
	const std::map<Square, Piece> expectedPositions = {
		{Square::fromRankAndFile(8, 'a'), {PieceType::Rook,   Color::Black}},
		{Square::fromRankAndFile(8, 'b'), {PieceType::Knight, Color::Black}},
		{Square::fromRankAndFile(8, 'c'), {PieceType::Bishop, Color::Black}},
		{Square::fromRankAndFile(8, 'd'), {PieceType::Queen,  Color::Black}},
		{Square::fromRankAndFile(8, 'e'), {PieceType::King,   Color::Black}},
		{Square::fromRankAndFile(8, 'f'), {PieceType::Bishop, Color::Black}},
		{Square::fromRankAndFile(8, 'g'), {PieceType::Knight, Color::Black}},
		{Square::fromRankAndFile(8, 'h'), {PieceType::Rook,   Color::Black}},
		{Square::fromRankAndFile(7, 'a'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'b'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'c'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'd'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'e'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'f'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'g'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'h'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(1, 'a'), {PieceType::Rook,   Color::White}},
		{Square::fromRankAndFile(1, 'b'), {PieceType::Knight, Color::White}},
		{Square::fromRankAndFile(1, 'c'), {PieceType::Bishop, Color::White}},
		{Square::fromRankAndFile(1, 'd'), {PieceType::Queen,  Color::White}},
		{Square::fromRankAndFile(1, 'e'), {PieceType::King,   Color::White}},
		{Square::fromRankAndFile(1, 'f'), {PieceType::Bishop, Color::White}},
		{Square::fromRankAndFile(1, 'g'), {PieceType::Knight, Color::White}},
		{Square::fromRankAndFile(1, 'h'), {PieceType::Rook,   Color::White}},
		{Square::fromRankAndFile(2, 'a'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'b'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'c'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'd'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'e'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'f'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'g'), {PieceType::Pawn,   Color::White}},
		{Square::fromRankAndFile(2, 'h'), {PieceType::Pawn,   Color::White}},
	};

	EXPECT_EQ(game.currentStage().board().occupiedSquares(), expectedPositions);
}

TEST(GameCreationTest, GameCreationFromPosition1) {
	const Game game = createGameFromFen(
			"5rk1/3Q1p1p/6p1/8/3B4/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Playing);

	EXPECT_THROW_CUSTOM(game.drawReason(), IllegalStateException);

	const auto& history = game.history();

	EXPECT_EQ(history.size(), 0);
	EXPECT_EQ(game.activeColor(), Color::Black);
	EXPECT_EQ(game.reasonToClaimDraw(), std::optional<DrawReason>());

	// Validate piece positions
	const std::map<Square, Piece> expectedPositions = {
		{Square::fromRankAndFile(8, 'f'), {PieceType::Rook,   Color::Black}},
		{Square::fromRankAndFile(8, 'g'), {PieceType::King,   Color::Black}},
		{Square::fromRankAndFile(7, 'd'), {PieceType::Queen,  Color::White}},
		{Square::fromRankAndFile(7, 'f'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(7, 'h'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(6, 'g'), {PieceType::Pawn,   Color::Black}},
		{Square::fromRankAndFile(4, 'd'), {PieceType::Bishop, Color::White}},
		{Square::fromRankAndFile(3, 'e'), {PieceType::King,   Color::White}},
	};

	EXPECT_EQ(game.currentStage().board().occupiedSquares(), expectedPositions);

}

TEST(GameCreationTest, GameCreationFromPositionInCheckmate) {
	const Game game = createGameFromFen(
			"6kr/5Q1p/3N2p1/8/8/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::WhiteWon);
}

TEST(GameCreationTest, GameCreationFromPositionInStalemate) {
	const Game game = createGameFromFen(
			"7k/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Drawn);
	EXPECT_EQ(game.drawReason(), DrawReason::StaleMate);
}

TEST(GameCreationTest, GameCreationWithTooManyKings) {
	EXPECT_THROW_CUSTOM(
			createGameFromFen("5kk1/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1"),
			std::invalid_argument);
}

TEST(GameCreationTest, GameCreationWithTooFewKings) {
	EXPECT_THROW_CUSTOM(
			createGameFromFen("8/5Qr1/5Q2/5B2/8/4K3/8/8 b - - 0 1"),
			std::invalid_argument);
}

TEST(GameCreationTest, GameCreationActiveSideAlreadyChecking) {
	EXPECT_THROW_CUSTOM(
			createGameFromFen("k4n2/5n1K/8/8/8/8/8/6r1 b - - 0 1"),
			std::invalid_argument);
}
