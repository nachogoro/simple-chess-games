#include "TestUtils.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

namespace
{
	const PieceMove whiteKnightForward = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const PieceMove whiteKnightBack = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(3, 'f'),
			Square::fromRankAndFile(1, 'g'));

	const PieceMove blackKnightForward = PieceMove::regularMove(
			{PieceType::Knight, Color::Black},
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(6, 'f'));

	const PieceMove blackKnightBack = PieceMove::regularMove(
			{PieceType::Knight, Color::Black},
			Square::fromRankAndFile(6, 'f'),
			Square::fromRankAndFile(8, 'g'));

	// One round = forward+forward+back+back, returning to the starting position.
	const std::vector<PieceMove> oneRound = {
		whiteKnightForward, blackKnightForward,
		whiteKnightBack,    blackKnightBack
	};

	Game playRounds(const Game& game, int rounds)
	{
		Game current = game;
		for (int r = 0; r < rounds; ++r)
		{
			for (const auto& move : oneRound)
			{
				current = makeMove(current, move);
			}
		}
		return current;
	}
}

TEST(DrawDetectionTest, OfferDraw) {
	const Game startingGame = createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = makeMove(startingGame, knightMove, true);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GameState::Playing);
	EXPECT_EQ(!!updated.reasonToClaimDraw(), true);
	EXPECT_EQ(*updated.reasonToClaimDraw(), DrawReason::OfferedAndAccepted);
}

TEST(DrawDetectionTest, OfferDrawAndAccept) {
	const Game startingGame = createNewGame();

	const PieceMove knightMove = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	// Offer draw
	const Game updated = makeMove(startingGame, knightMove, true);
	const Game drawnGame = claimDraw(updated);

	EXPECT_EQ(drawnGame.gameState(), GameState::Drawn);
	EXPECT_EQ(drawnGame.drawReason(), DrawReason::OfferedAndAccepted);
}

TEST(DrawDetectionTest, OfferDrawAndReject) {
	const Game startingGame = createNewGame();

	const PieceMove whiteMove = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const PieceMove blackMove = PieceMove::regularMove(
			{PieceType::Knight, Color::Black},
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(6, 'f'));

	// Offer draw
	const Game withDrawOffer = makeMove(startingGame, whiteMove, true);

	// ... and ignore it
	const Game withDrawRejected = makeMove(withDrawOffer, blackMove, false);

	EXPECT_EQ(withDrawOffer.gameState(), GameState::Playing);
	EXPECT_EQ(*withDrawOffer.reasonToClaimDraw(), DrawReason::OfferedAndAccepted);
	EXPECT_EQ(withDrawRejected.gameState(), GameState::Playing);
	EXPECT_EQ(!!withDrawRejected.reasonToClaimDraw(), false);
}

TEST(DrawDetectionTest, Stalemate) {
	const Game startingGame = createGameFromFen(
			"8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1");

	const PieceMove causeStaleMate = PieceMove::regularMove(
			{PieceType::Rook, Color::Black},
			Square::fromRankAndFile(5, 'h'),
			Square::fromRankAndFile(4, 'h'));

	const Game updated = makeMove(startingGame, causeStaleMate, false);

	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);
	EXPECT_EQ(updated.gameState(), GameState::Drawn);
	EXPECT_EQ(updated.drawReason(), DrawReason::StaleMate);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKing) {
	const Game startingGame = createGameFromFen(
			"8/3k4/8/4p3/3K4/8/8/8 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove pawnCapture = PieceMove::regularMove(
			{PieceType::King, Color::White},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'e'));

	const Game withNoMaterial = makeMove(startingGame, pawnCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GameState::Drawn);
	EXPECT_EQ(withNoMaterial.drawReason(), DrawReason::InsufficientMaterial);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndBishop) {
	const Game startingGame = createGameFromFen(
			"3k4/4R3/2B5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove rookCapture = PieceMove::regularMove(
			{PieceType::King, Color::Black},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'e'));

	const Game withNoMaterial = makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GameState::Drawn);
	EXPECT_EQ(withNoMaterial.drawReason(), DrawReason::InsufficientMaterial);
}

TEST(DrawDetectionTest, InsufficientMaterialKingvsKingAndKnight) {
	const Game startingGame = createGameFromFen(
			"3k4/3R4/2N5/8/3K4/8/8/8 b - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove rookCapture = PieceMove::regularMove(
			{PieceType::King, Color::Black},
			Square::fromRankAndFile(8, 'd'),
			Square::fromRankAndFile(7, 'd'));

	const Game withNoMaterial = makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GameState::Drawn);
	EXPECT_EQ(withNoMaterial.drawReason(), DrawReason::InsufficientMaterial);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndSameColoredBishop) {
	const Game startingGame = createGameFromFen(
			"3k4/2b5/8/3r4/3K4/8/8/6B1 w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove rookCapture = PieceMove::regularMove(
			{PieceType::King, Color::White},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game withNoMaterial = makeMove(startingGame, rookCapture);

	EXPECT_EQ(withNoMaterial.gameState(), GameState::Drawn);
	EXPECT_EQ(withNoMaterial.drawReason(), DrawReason::InsufficientMaterial);
}

TEST(DrawDetectionTest, InsufficientMaterialKingAndBishopvsKingAndOppositeColorBishop) {
	const Game startingGame = createGameFromFen(
			"3k4/2b5/8/3r4/3K4/8/8/7B w - - 0 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove rookCapture = PieceMove::regularMove(
			{PieceType::King, Color::White},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'd'));

	const Game stillEnoughMaterial = makeMove(startingGame, rookCapture);

	EXPECT_EQ(stillEnoughMaterial.gameState(), GameState::Playing);
}

TEST(DrawDetectionTest, OpponentInsufficientMaterialKingvsKingAndQueen) {
	// White has King + Queen, Black has only King
	// White (active) can claim a draw because opponent can't win
	const Game game = createGameFromFen(
			"3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Playing);
	EXPECT_EQ(!!game.reasonToClaimDraw(), true);
	EXPECT_EQ(*game.reasonToClaimDraw(), DrawReason::OpponentInsufficientMaterial);

	const Game drawnGame = claimDraw(game);
	EXPECT_EQ(drawnGame.gameState(), GameState::Drawn);
	EXPECT_EQ(drawnGame.drawReason(), DrawReason::OpponentInsufficientMaterial);
}

TEST(DrawDetectionTest, OpponentInsufficientMaterialKingvsKingAndRook) {
	// White has King + Rook, Black has only King
	const Game game = createGameFromFen(
			"3k4/8/8/8/8/8/3K4/3R4 w - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Playing);
	EXPECT_EQ(!!game.reasonToClaimDraw(), true);
	EXPECT_EQ(*game.reasonToClaimDraw(), DrawReason::OpponentInsufficientMaterial);
}

TEST(DrawDetectionTest, OpponentInsufficientMaterialNotClaimableWhenOpponentHasPieces) {
	// Both sides have pieces beyond king — no claim
	const Game game = createGameFromFen(
			"3k4/4p3/8/8/8/8/3K4/3Q4 w - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Playing);
	EXPECT_EQ(!!game.reasonToClaimDraw(), false);
}

TEST(DrawDetectionTest, OpponentInsufficientMaterialNeverAutomatic) {
	// Even with Automatic enforcement, this should not auto-draw
	const Game game = createGameFromFen(
			"3k4/8/8/8/8/8/3K4/3Q4 w - - 0 1");

	EXPECT_EQ(game.gameState(), GameState::Playing);
	EXPECT_EQ(!!game.reasonToClaimDraw(), true);
	EXPECT_EQ(*game.reasonToClaimDraw(), DrawReason::OpponentInsufficientMaterial);
}

TEST(DrawDetectionTest, InvalidClaim) {
	const Game startingGame = createNewGame();

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);
	EXPECT_THROW_CUSTOM(claimDraw(startingGame), IllegalStateException);
}

TEST(DrawDetectionTest, NFoldRepetition) {
	const Game startingGame = createNewGame();

	const Game move1 = makeMove(startingGame, whiteKnightForward);
	EXPECT_EQ(!!move1.reasonToClaimDraw(), false);
	const Game move1Black = makeMove(move1, blackKnightForward);
	EXPECT_EQ(!!move1Black.reasonToClaimDraw(), false);
	const Game move2 = makeMove(move1Black, whiteKnightBack);
	EXPECT_EQ(!!move2.reasonToClaimDraw(), false);
	const Game move2Black = makeMove(move2, blackKnightBack);
	EXPECT_EQ(!!move2Black.reasonToClaimDraw(), false);

	const Game move3 = makeMove(move2Black, whiteKnightForward);
	EXPECT_EQ(!!move3.reasonToClaimDraw(), false);
	const Game move3Black = makeMove(move3, blackKnightForward);
	EXPECT_EQ(!!move3Black.reasonToClaimDraw(), false);
	const Game move4 = makeMove(move3Black, whiteKnightBack);
	// At this point, black could play blackKnightBack, which would cause
	// 3-fold repetition. Therefore, black can claim a draw at this point, even
	// before making the move.
	EXPECT_EQ(!!move4.reasonToClaimDraw(), true);

	// Complete the remaining rounds to reach 5-fold repetition.
	// move4 is mid-cycle (white just moved back), so we finish
	// this round and play 2 more full rounds from the reset position.
	const Game endOfRound2 = makeMove(move4, blackKnightBack);
	const Game fiveFold = playRounds(endOfRound2, 2);

	EXPECT_EQ(fiveFold.gameState(), GameState::Drawn);
	EXPECT_EQ(fiveFold.drawReason(), DrawReason::FiveFoldRepetition);
}

TEST(DrawDetectionTest, FiftyMoveRule) {
	const Game startingGame = createGameFromFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 98 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), false);

	// Available when the move would be the 50th
	const Game oneBeforeFifty = makeMove(startingGame,
			PieceMove::regularMove(
				{PieceType::King, Color::White},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(oneBeforeFifty.gameState(), GameState::Playing);
	EXPECT_EQ(!!oneBeforeFifty.reasonToClaimDraw(), true);
	EXPECT_EQ(*oneBeforeFifty.reasonToClaimDraw(), DrawReason::FiftyMoveRule);

	// Test not automatically claimed at fifty
	const Game exactlyFifty = makeMove(oneBeforeFifty,
			PieceMove::regularMove(
				{PieceType::Bishop, Color::Black},
				Square::fromRankAndFile(7, 'c'),
				Square::fromRankAndFile(6, 'b')));

	EXPECT_EQ(exactlyFifty.gameState(), GameState::Playing);
	EXPECT_EQ(!!exactlyFifty.reasonToClaimDraw(), true);
	EXPECT_EQ(*exactlyFifty.reasonToClaimDraw(), DrawReason::FiftyMoveRule);

	// Not automatically claimed after fifty
	const Game afterFifty = makeMove(exactlyFifty,
			PieceMove::regularMove(
				{PieceType::Bishop, Color::White},
				Square::fromRankAndFile(1, 'h'),
				Square::fromRankAndFile(2, 'g')));

	EXPECT_EQ(afterFifty.gameState(), GameState::Playing);
	EXPECT_EQ(!!afterFifty.reasonToClaimDraw(), true);
	EXPECT_EQ(*afterFifty.reasonToClaimDraw(), DrawReason::FiftyMoveRule);
}

TEST(DrawDetectionTest, SeventyFiveMoveRule) {
	const Game startingGame = createGameFromFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), true);
	EXPECT_EQ(*startingGame.reasonToClaimDraw(), DrawReason::FiftyMoveRule);

	const Game seventyFiveFullMoves = makeMove(startingGame,
			PieceMove::regularMove(
				{PieceType::King, Color::White},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	EXPECT_EQ(seventyFiveFullMoves.gameState(), GameState::Drawn);
	EXPECT_EQ(seventyFiveFullMoves.drawReason(), DrawReason::SeventyFiveMoveRule);
}

TEST(DrawDetectionTest, SeventyFiveMoveRuleCheckmateOverride) {
	// Position where black can deliver checkmate on the 75th move
	const Game startingGame = createGameFromFen(
			"1r3k2/8/8/8/8/8/4PPPP/6K1 b - - 149 1");

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);
	EXPECT_EQ(!!startingGame.reasonToClaimDraw(), true);
	EXPECT_EQ(*startingGame.reasonToClaimDraw(), DrawReason::FiftyMoveRule);

	// The 75th move delivers checkmate - checkmate should take precedence over draw
	const Game checkmateOn75th = makeMove(startingGame,
			PieceMove::regularMove(
				{PieceType::Rook, Color::Black},
				Square::fromRankAndFile(8, 'b'),
				Square::fromRankAndFile(1, 'b')));

	// Checkmate takes precedence over the 75-move rule
	EXPECT_EQ(checkmateOn75th.gameState(), GameState::BlackWon);
}

// ===== DrawEnforcement::ClaimOnly tests =====

TEST(DrawDetectionTest, ClaimOnlyInsufficientMaterialNotAutoDrawn) {
	const Game startingGame = createGameFromFen(
			"8/3k4/8/4p3/3K4/8/8/8 w - - 0 1",
			DrawEnforcement::ClaimOnly);

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const PieceMove pawnCapture = PieceMove::regularMove(
			{PieceType::King, Color::White},
			Square::fromRankAndFile(4, 'd'),
			Square::fromRankAndFile(5, 'e'));

	const Game withNoMaterial = makeMove(startingGame, pawnCapture);

	// With ClaimOnly, the game should NOT auto-draw
	EXPECT_EQ(withNoMaterial.gameState(), GameState::Playing);
	EXPECT_EQ(!!withNoMaterial.reasonToClaimDraw(), true);
	EXPECT_EQ(*withNoMaterial.reasonToClaimDraw(), DrawReason::InsufficientMaterial);

	// But it should still be claimable
	const Game drawnGame = claimDraw(withNoMaterial);
	EXPECT_EQ(drawnGame.gameState(), GameState::Drawn);
	EXPECT_EQ(drawnGame.drawReason(), DrawReason::InsufficientMaterial);
}

TEST(DrawDetectionTest, ClaimOnlySeventyFiveMoveRuleNotAutoDrawn) {
	const Game startingGame = createGameFromFen(
			"3k4/2b5/8/3r4/8/8/3K4/7B w - - 149 1",
			DrawEnforcement::ClaimOnly);

	EXPECT_EQ(startingGame.gameState(), GameState::Playing);

	const Game seventyFiveFullMoves = makeMove(startingGame,
			PieceMove::regularMove(
				{PieceType::King, Color::White},
				Square::fromRankAndFile(2, 'd'),
				Square::fromRankAndFile(2, 'c')));

	// With ClaimOnly, the game should NOT auto-draw
	EXPECT_EQ(seventyFiveFullMoves.gameState(), GameState::Playing);
	EXPECT_EQ(!!seventyFiveFullMoves.reasonToClaimDraw(), true);
	EXPECT_EQ(*seventyFiveFullMoves.reasonToClaimDraw(), DrawReason::SeventyFiveMoveRule);

	// But it should still be claimable
	const Game drawnGame = claimDraw(seventyFiveFullMoves);
	EXPECT_EQ(drawnGame.gameState(), GameState::Drawn);
	EXPECT_EQ(drawnGame.drawReason(), DrawReason::SeventyFiveMoveRule);
}

TEST(DrawDetectionTest, ClaimOnlyFiveFoldRepetitionNotAutoDrawn) {
	const Game startingGame = createNewGame(DrawEnforcement::ClaimOnly);

	// 4 full rounds = 16 half-moves, reaching the starting position 5 times
	const Game fiveFold = playRounds(startingGame, 4);

	// With ClaimOnly, fivefold repetition should NOT auto-draw
	EXPECT_EQ(fiveFold.gameState(), GameState::Playing);
	EXPECT_EQ(!!fiveFold.reasonToClaimDraw(), true);
	EXPECT_EQ(*fiveFold.reasonToClaimDraw(), DrawReason::FiveFoldRepetition);

	// But it should still be claimable
	const Game drawnGame = claimDraw(fiveFold);
	EXPECT_EQ(drawnGame.gameState(), GameState::Drawn);
	EXPECT_EQ(drawnGame.drawReason(), DrawReason::FiveFoldRepetition);
}

TEST(DrawDetectionTest, ClaimOnlyStalemateStillAutoDrawn) {
	const Game startingGame = createGameFromFen(
			"8/5b2/1q6/3R3r/2K1N3/2P5/4k3/8 b - - 0 1",
			DrawEnforcement::ClaimOnly);

	const PieceMove causeStaleMate = PieceMove::regularMove(
			{PieceType::Rook, Color::Black},
			Square::fromRankAndFile(5, 'h'),
			Square::fromRankAndFile(4, 'h'));

	const Game updated = makeMove(startingGame, causeStaleMate, false);

	// Stalemate should ALWAYS be enforced, even in ClaimOnly mode
	EXPECT_EQ(updated.gameState(), GameState::Drawn);
	EXPECT_EQ(updated.drawReason(), DrawReason::StaleMate);
}

TEST(DrawDetectionTest, ClaimOnlyCheckmateStillEnforced) {
	// Position where black can deliver checkmate on the 75th move
	const Game startingGame = createGameFromFen(
			"1r3k2/8/8/8/8/8/4PPPP/6K1 b - - 149 1",
			DrawEnforcement::ClaimOnly);

	const Game checkmateOn75th = makeMove(startingGame,
			PieceMove::regularMove(
				{PieceType::Rook, Color::Black},
				Square::fromRankAndFile(8, 'b'),
				Square::fromRankAndFile(1, 'b')));

	// Checkmate is always enforced
	EXPECT_EQ(checkmateOn75th.gameState(), GameState::BlackWon);
}

TEST(DrawDetectionTest, DrawEnforcementPreservedAcrossMoves) {
	const Game game = createNewGame(DrawEnforcement::ClaimOnly);
	EXPECT_EQ(game.drawEnforcement(), DrawEnforcement::ClaimOnly);

	const PieceMove knightMove = PieceMove::regularMove(
			{PieceType::Knight, Color::White},
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(3, 'f'));

	const Game afterMove = makeMove(game, knightMove);
	EXPECT_EQ(afterMove.drawEnforcement(), DrawEnforcement::ClaimOnly);
}

TEST(DrawDetectionTest, DefaultDrawEnforcementIsAutomatic) {
	const Game game = createNewGame();
	EXPECT_EQ(game.drawEnforcement(), DrawEnforcement::Automatic);
}
