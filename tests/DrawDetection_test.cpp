#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

TEST(DrawDetectionTest, OfferDraw) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = gameMgr.makeMove(startingGame, knightMove, true);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(*updated.reasonToClaimDraw(), DRAW_REASON_OFFERED_AND_ACCEPTED);
}

TEST(DrawDetectionTest, OfferDrawAndAccept) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = gameMgr.makeMove(startingGame, knightMove, true);
	const Game drawnGame = gameMgr.claimDraw(updated);

	EXPECT_EQ(drawnGame.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(drawnGame.drawReason(), DRAW_REASON_OFFERED_AND_ACCEPTED);
}

TEST(DrawDetectionTest, OfferDrawAndReject) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();

	const PieceMove whiteMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_WHITE},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const PieceMove blackMove = PieceMove::regularMove(
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(6, 'f'));

	// Offer draw
	const Game withDrawOffer = gameMgr.makeMove(startingGame, whiteMove, true);

	// ... and ignore it
	const Game withDrawRejected = gameMgr.makeMove(withDrawOffer, blackMove, false);

	EXPECT_EQ(withDrawOffer.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(*withDrawOffer.reasonToClaimDraw(), DRAW_REASON_OFFERED_AND_ACCEPTED);
	EXPECT_EQ(withDrawRejected.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!withDrawRejected.reasonToClaimDraw(), false);
}

TEST(DrawDetectionTest, Stalemate) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1");

	const PieceMove causeStaleMate = PieceMove::regularMove(
			{TYPE_ROOK, COLOR_BLACK},
			Square::fromRankAndFile(5, 'h'),
			Square::fromRankAndFile(4, 'h'));

	const Game updated = gameMgr.makeMove(startingGame, causeStaleMate, false);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(updated.drawReason(), DRAW_REASON_STALEMATE);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKing) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"8/3k4/8/4p3/3K4/8/8/8 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove pawnCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'e'));

	const Game withNoMaterial = gameMgr.makeMove(startingGame, pawnCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndBishop) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/4R3/2B5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'e'));

	const Game withNoMaterial = gameMgr.makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndKnight) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/3R4/2N5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'd'));

	const Game withNoMaterial = gameMgr.makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndSameColoredBishop) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/2b5/8/3r4/3K4/8/8/6B1 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game withNoMaterial = gameMgr.makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(withNoMaterial.drawReason(), DRAW_REASON_INSUFFICIENT_MATERIAL);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndOppositeColorBishop) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/2b5/8/3r4/3K4/8/8/7B w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);

	const PieceMove rookCapture = PieceMove::regularMove(
			{TYPE_KING, COLOR_WHITE},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game stillEnoughMaterial = gameMgr.makeMove(startingGame, rookCapture);

	EXPECT_EQ(stillEnoughMaterial.gameState(), GAME_STATE_PLAYING);
}

TEST(DrawDetectionTest, InvalidClaim) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_THROW_CUSTOM(gameMgr.claimDraw(startingGame), IllegalStateException);
}

TEST(DrawDetectionTest, NFoldRepetition) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createNewGame();

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

	const Game move1 = gameMgr.makeMove(startingGame, whiteKnightForward);
	EXPECT_EQ(!!move1.reasonToClaimDraw(), false);
	const Game move1Black = gameMgr.makeMove(move1, blackKnightForward);
	EXPECT_EQ(!!move1Black.reasonToClaimDraw(), false);
	const Game move2 = gameMgr.makeMove(move1Black, whiteKnightBack);
	EXPECT_EQ(!!move2.reasonToClaimDraw(), false);
	const Game move2Black = gameMgr.makeMove(move2, blackKnightBack);
	EXPECT_EQ(!!move2Black.reasonToClaimDraw(), false);

	const Game move3 = gameMgr.makeMove(move2Black, whiteKnightForward);
	EXPECT_EQ(!!move3.reasonToClaimDraw(), false);
	const Game move3Black = gameMgr.makeMove(move3, blackKnightForward);
	EXPECT_EQ(!!move3Black.reasonToClaimDraw(), false);
	const Game move4 = gameMgr.makeMove(move3Black, whiteKnightBack);
	// At this point, black could play blackKnightBack, which would cause
	// 3-fold repetition. Therefore, black can claim a draw at this point, even
	// before making the move.
	EXPECT_EQ(!!move4.reasonToClaimDraw(), true);

	const Game fiveFold
		= gameMgr.makeMove(
				gameMgr.makeMove(
					gameMgr.makeMove(
						gameMgr.makeMove(
							gameMgr.makeMove(
								gameMgr.makeMove(
									gameMgr.makeMove(
										gameMgr.makeMove(
											gameMgr.makeMove(
												move4,
												blackKnightBack),
											whiteKnightForward),
										blackKnightForward),
									whiteKnightBack),
								blackKnightBack),
							whiteKnightForward),
						blackKnightForward),
					whiteKnightBack),
				blackKnightBack);

	EXPECT_EQ(fiveFold.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(fiveFold.drawReason(), DRAW_REASON_FIVE_FOLD_REPETITION);
}

TEST(DrawDetectionTest, FiftyMoveRule) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 99 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);

	const Game fiftyFullMoves = gameMgr.makeMove(startingGame, 
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(fiftyFullMoves.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!fiftyFullMoves.reasonToClaimDraw(), true);
	EXPECT_EQ(*fiftyFullMoves.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);

	const Game fiftyOneFullMoves = gameMgr.makeMove(fiftyFullMoves, 
			PieceMove::regularMove(
				{TYPE_BISHOP, COLOR_BLACK},
				Square::fromRankAndFile(7, 'c'),
				Square::fromRankAndFile(6, 'b')));

	EXPECT_EQ(fiftyOneFullMoves.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!fiftyOneFullMoves.reasonToClaimDraw(), true);
	EXPECT_EQ(*fiftyOneFullMoves.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);
}

TEST(DrawDetectionTest, SeventyFiveMoveRule) {
	const GameManager gameMgr;
	const Game startingGame = gameMgr.createGameFromFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1");

	EXPECT_EQ(startingGame.gameState(), GAME_STATE_PLAYING);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), true);
	EXPECT_EQ(*startingGame.reasonToClaimDraw(), DRAW_REASON_50_MOVE_RULE);

	const Game seventyFiveFullMoves = gameMgr.makeMove(startingGame, 
			PieceMove::regularMove(
				{TYPE_KING, COLOR_WHITE},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(seventyFiveFullMoves.gameState(), GAME_STATE_DRAWN);
	EXPECT_EQ(seventyFiveFullMoves.drawReason(), DRAW_REASON_75_MOVE_RULE);
}
