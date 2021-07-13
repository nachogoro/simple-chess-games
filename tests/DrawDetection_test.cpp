#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(DrawDetectionTest, OfferDraw) {
	const Game startingGame = Game::createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = startingGame.makeMove(knightMove, true);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(*updated.reasonToClaimDraw(), DRAW_REASON_OFFERED_AND_ACCEPTED);
}

TEST(DrawDetectionTest, OfferDrawAndAccept) {
	const Game startingGame = Game::createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = startingGame.makeMove(knightMove, true);
	const Game drawnGame = updated.claimDraw();

	EXPECT_EQ(drawnGame.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(drawnGame.drawReason(), DRAW_REASON_OFFERED_AND_ACCEPTED);
}

TEST(DrawDetectionTest, OfferDrawAndReject) {
	const Game startingGame = Game::createNewGame();

	const PieceMove whiteMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const PieceMove blackMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(6, 'f'));

	// Offer draw
	const Game withDrawOffer = startingGame.makeMove(whiteMove, true);

	// ... and ignore it
	const Game withDrawRejected = withDrawOffer.makeMove(blackMove, false);

	EXPECT_EQ(withDrawOffer.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(*withDrawOffer.reasonToClaimDraw(), DRAW_REASON_OFFERED_AND_ACCEPTED);
	EXPECT_EQ(withDrawRejected.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!withDrawRejected.reasonToClaimDraw(), false);
}

TEST(DrawDetectionTest, Stalemate) {
	const Game startingGame = Game::createGameFromStartingFen(
			"8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1");

	const PieceMove causeStaleMate = PieceMove::regularMove(
			{TYPE_ROOK, COLOR_BLACK},
			Square::fromRankAndFile(5, 'h'),
			Square::fromRankAndFile(4, 'h'));

	const Game updated = startingGame.makeMove(causeStaleMate, false);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(updated.drawReason(), DRAW_REASON_STALEMATE);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKing) {
	const Game startingGame = Game::createGameFromStartingFen(
			"8/3k4/8/4p3/3K4/8/8/8 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove pawnCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'e'));

	const Game withNoMaterial = startingGame.makeMove(pawnCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndBishop) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/4R3/2B5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'e'));

	const Game withNoMaterial = startingGame.makeMove(rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndKnight) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/3R4/2N5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'd'));

	const Game withNoMaterial = startingGame.makeMove(rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndSameColoredBishop) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/2b5/8/3r4/3K4/8/8/6B1 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game withNoMaterial = startingGame.makeMove(rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndOppositeColorBishop) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/2b5/8/3r4/3K4/8/8/7B w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game stillEnoughMaterial = startingGame.makeMove(rookCapture);

	EXPECT_EQ(stillEnoughMaterial.gameState(), GAME_STATE_PLAYING);
}

TEST(DrawDetectionTest, InvalidClaim) {
	const Game startingGame = Game::createNewGame();

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_THROW_CUSTOM(startingGame.claimDraw(), IllegalStateException);
}

TEST(DrawDetectionTest, NFoldRepetition) {
	const Game startingGame = Game::createNewGame();

	const PieceMove whiteKnightForward = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const PieceMove whiteKnightBack = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(3, 'f'),
			Square::fromRankAndFile(1, 'g'));

	const PieceMove blackKnightForward = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(6, 'f'));

	const PieceMove blackKnightBack = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(6, 'f'),
			Square::fromRankAndFile(8, 'g'));

	const Game move1 = startingGame.makeMove(whiteKnightForward);
	EXPECT_EQ(!!move1.reasonToClaimDraw(), false);
	const Game move1Black = move1.makeMove(blackKnightForward);
	EXPECT_EQ(!!move1Black.reasonToClaimDraw(), false);
	const Game move2 = move1Black.makeMove(whiteKnightBack);
	EXPECT_EQ(!!move2.reasonToClaimDraw(), false);
	const Game move2Black = move2.makeMove(blackKnightBack);
	EXPECT_EQ(!!move2Black.reasonToClaimDraw(), false);

	const Game move3 = move2Black.makeMove(whiteKnightForward);
	EXPECT_EQ(!!move3.reasonToClaimDraw(), false);
	const Game move3Black = move3.makeMove(blackKnightForward);
	EXPECT_EQ(!!move3Black.reasonToClaimDraw(), false);
	const Game move4 = move3Black.makeMove(whiteKnightBack);
	// At this point, black could play blackKnightBack, which would cause
	// 3-fold repetition. Therefore, black can claim a draw at this point, even
	// before making the move.
	EXPECT_EQ(!!move4.reasonToClaimDraw(), true);

	const Game fiveFold
		= move4.makeMove(blackKnightBack).makeMove(whiteKnightForward)
		.makeMove(blackKnightForward).makeMove(whiteKnightBack)
		.makeMove(blackKnightBack).makeMove(whiteKnightForward)
		.makeMove(blackKnightForward).makeMove(whiteKnightBack)
		.makeMove(blackKnightBack);

	EXPECT_EQ(fiveFold.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(fiveFold.drawReason(), DRAW_REASON_FIVE_FOLD_REPETITION);
}

TEST(DrawDetectionTest, FiftyMoveRule) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 99 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);

	const Game fiftyFullMoves = startingGame.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(fiftyFullMoves.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!fiftyFullMoves.reasonToClaimDraw(), true);
	EXPECT_EQ(*fiftyFullMoves.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);

	const Game fiftyOneFullMoves = fiftyFullMoves.makeMove(
			PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_BLACK},
				Square::fromRankAndFile(7, 'c'),
				Square::fromRankAndFile(6, 'b')));

	EXPECT_EQ(fiftyOneFullMoves.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!fiftyOneFullMoves.reasonToClaimDraw(), true);
	EXPECT_EQ(*fiftyOneFullMoves.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);
}

TEST(DrawDetectionTest, SeventyFiveMoveRule) {
	const Game startingGame = Game::createGameFromStartingFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), true);
	EXPECT_EQ(*startingGame.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);

	const Game seventyFiveFullMoves = startingGame.makeMove(
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(seventyFiveFullMoves.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(seventyFiveFullMoves.drawReason(), DRAW_REASON_75_MOVE_RULE);
}
