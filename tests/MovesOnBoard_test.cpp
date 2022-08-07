#include <gtest/gtest.h>
#include "GameManager.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

namespace
{
	enum class Difference
	{
		PieceAppeared,
		PieceDisappeared,
		PieceReplaced
	};

	struct Effect
	{
		public:
			Effect(const Piece& piece, const Difference diff)
				: piece(piece),
				diff(diff)
			{
			}

			const Piece piece;
			const Difference diff;

			bool operator==(const Effect& rhs) const
			{
				return piece == rhs.piece && diff == rhs.diff;
			}
	};

	std::map<Square, Effect> compareBoards(
			const Board& after,
			const Board& before)
	{
		std::map<Square, Effect> result;

		for (const auto& entry : after.occupiedSquares())
		{
			const std::optional<Piece> inBefore = before.pieceAt(entry.first);
			if (!inBefore)
			{
				// This square was empty in before but not in after, a piece
				// appeared
				result.insert({entry.first, {entry.second, Difference::PieceAppeared}});
			}
			else if (*inBefore != entry.second)
			{
				// The contents of this square have changed
				result.insert({entry.first, {entry.second, Difference::PieceReplaced}});
			}
		}

		for (const auto& entry : before.occupiedSquares())
		{
			const std::optional<Piece> inAfter = after.pieceAt(entry.first);
			if (!inAfter)
			{
				// This square was empty in before but not in after, a piece
				// appeared
				result.insert({entry.first, {entry.second, Difference::PieceDisappeared}});
			}
		}

		return result;
	}

	void regularNonCaptureTest(
			const Game& startingGame,
			const Piece& piece,
			const Square& src,
			const Square& dst)
	{
		const Game afterMove = GameManager().makeMove(
				startingGame,
				PieceMove::regularMove(
					piece,
					src,
					dst));

		const std::map<Square, Effect> comparison = ::compareBoards(
				afterMove.currentStage().board(),
				startingGame.currentStage().board());

		const std::map<Square, Effect> expected =
		{
			{
				src, {piece, Difference::PieceDisappeared}
			},

			{
				dst, {piece, Difference::PieceAppeared}
			}
		};

		EXPECT_EQ(comparison, expected);
	}

	void regularCaptureTest(
			const Game& startingGame,
			const Piece& piece,
			const Square& src,
			const Square& dst)
	{
		const Game afterMove = GameManager().makeMove(
				startingGame,
				PieceMove::regularMove(
					piece,
					src,
					dst));

		const std::map<Square, Effect> comparison = ::compareBoards(
				afterMove.currentStage().board(),
				startingGame.currentStage().board());

		const std::map<Square, Effect> expected =
		{
			{
				src, {piece, Difference::PieceDisappeared}
			},

			{
				dst, {piece, Difference::PieceReplaced}
			}
		};

		EXPECT_EQ(comparison, expected);
	}

	void castlingTest(
			const Game& startingGame,
			const Color castlingColor,
			const Square& kingSrc,
			const Square& kingDst,
			const Square& rookSrc,
			const Square& rookDst)
	{
		const Piece king = {PieceType::King, castlingColor};
		const Piece rook = {PieceType::Rook, castlingColor};

		const Game afterPawnMove = GameManager().makeMove(
				startingGame,
				PieceMove::regularMove(
					king,
					kingSrc,
					kingDst));

		const std::map<Square, Effect> comparison = ::compareBoards(
				afterPawnMove.currentStage().board(),
				startingGame.currentStage().board());

		const std::map<Square, Effect> expected =
		{
			{
				kingSrc, {king, Difference::PieceDisappeared}
			},

			{
				kingDst, {king, Difference::PieceAppeared}
			},

			{
				rookSrc, {rook, Difference::PieceDisappeared}
			},

			{
				rookDst, {rook, Difference::PieceAppeared}
			},
		};

		EXPECT_EQ(comparison, expected);
	}
}

TEST(MovesOnBoardTest, PawnOnceForward) {
	regularNonCaptureTest(
			GameManager().createNewGame(),
			{PieceType::Pawn, Color::White},
			Square::fromRankAndFile(2, 'f'),
			Square::fromRankAndFile(3, 'f'));
}

TEST(MovesOnBoardTest, PawnTwiceForward) {
	regularNonCaptureTest(
			GameManager().createNewGame(),
			{PieceType::Pawn, Color::White},
			Square::fromRankAndFile(2, 'a'),
			Square::fromRankAndFile(4, 'a'));
}

TEST(MovesOnBoardTest, PawnCapture) {
	regularCaptureTest(
			GameManager().createGameFromFen(
				"rn1qk2r/ppp2ppp/3p1n2/4p3/3P2b1/2N1P3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			{PieceType::Pawn, Color::Black},
			Square::fromRankAndFile(5, 'e'),
			Square::fromRankAndFile(4, 'd'));
}

TEST(MovesOnBoardTest, WhitePawnEnPassant) {
	const Game startingGame = GameManager().createGameFromFen(
			"rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

	const Square src = Square::fromRankAndFile(5, 'd');
	const Square dst = Square::fromRankAndFile(6, 'e');
	const Square squareOfCapturedPawn = Square::fromRankAndFile(5, 'e');

	const Piece piece = {PieceType::Pawn, Color::White};

	const Game afterPawnMove = GameManager().makeMove(
			startingGame,
			PieceMove::regularMove(
				{PieceType::Pawn, Color::White},
				src,
				dst));

	const std::map<Square, Effect> comparison = ::compareBoards(
			afterPawnMove.currentStage().board(),
			startingGame.currentStage().board());

	const std::map<Square, Effect> expected =
	{
		{
			src, {piece, Difference::PieceDisappeared}
		},

		{
			dst, {piece, Difference::PieceAppeared}
		},

		{
			squareOfCapturedPawn, {{PieceType::Pawn, Color::Black}, Difference::PieceDisappeared}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, BlackPawnEnPassant) {
	const Game startingGame = GameManager().createGameFromFen(
			"8/4k3/8/8/6pP/8/1K6/8 b - h3 0 1");

	const Square src = Square::fromRankAndFile(4, 'g');
	const Square dst = Square::fromRankAndFile(3, 'h');
	const Square squareOfCapturedPawn = Square::fromRankAndFile(4, 'h');

	const Piece piece = {PieceType::Pawn, Color::Black};

	const Game afterPawnMove = GameManager().makeMove(
			startingGame,
			PieceMove::regularMove(
				piece,
				src,
				dst));

	const std::map<Square, Effect> comparison = ::compareBoards(
			afterPawnMove.currentStage().board(),
			startingGame.currentStage().board());

	const std::map<Square, Effect> expected =
	{
		{
			src, {piece, Difference::PieceDisappeared}
		},

		{
			dst, {piece, Difference::PieceAppeared}
		},

		{
			squareOfCapturedPawn, {{PieceType::Pawn, Color::White}, Difference::PieceDisappeared}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, PawnPromotionNoCapture) {
	const Game startingGame = GameManager().createGameFromFen(
			"8/4k3/8/2q5/7P/2RQ4/1K4p1/8 b - - 0 1");

	const Square src = Square::fromRankAndFile(2, 'g');
	const Square dst = Square::fromRankAndFile(1, 'g');

	const Piece piece = {PieceType::Pawn, Color::Black};
	const Piece promoted = {PieceType::Queen, Color::Black};

	const Game afterPawnMove = GameManager().makeMove(
			startingGame,
			PieceMove::pawnPromotion(
				piece,
				src,
				dst,
				promoted.type()));

	const std::map<Square, Effect> comparison = ::compareBoards(
			afterPawnMove.currentStage().board(),
			startingGame.currentStage().board());

	const std::map<Square, Effect> expected =
	{
		{
			src, {piece, Difference::PieceDisappeared}
		},

		{
			dst, {promoted, Difference::PieceAppeared}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, PawnPromotionCapture) {
	const Game startingGame = GameManager().createGameFromFen(
			"2q5/1P2k3/8/8/8/2RQ4/1K4p1/8 w - - 0 1");

	const Square src = Square::fromRankAndFile(7, 'b');
	const Square dst = Square::fromRankAndFile(8, 'c');

	const Piece piece = {PieceType::Pawn, Color::White};
	const Piece promoted = {PieceType::Queen, Color::White};

	const Game afterPawnMove = GameManager().makeMove(
			startingGame,
			PieceMove::pawnPromotion(
				piece,
				src,
				dst,
				promoted.type()));

	const std::map<Square, Effect> comparison = ::compareBoards(
			afterPawnMove.currentStage().board(),
			startingGame.currentStage().board());

	const std::map<Square, Effect> expected =
	{
		{
			src, {piece, Difference::PieceDisappeared}
		},

		{
			dst, {promoted, Difference::PieceReplaced}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, KnightNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1"),
			{PieceType::Knight, Color::Black},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'b'));
}

TEST(MovesOnBoardTest, KnightCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1"),
			{PieceType::Knight, Color::Black},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'd'));
}

TEST(MovesOnBoardTest, BishopNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1"),
			{PieceType::Bishop, Color::White},
			Square::fromRankAndFile(3, 'd'),
			Square::fromRankAndFile(1, 'f'));
}

TEST(MovesOnBoardTest, BishopCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1"),
			{PieceType::Bishop, Color::White},
			Square::fromRankAndFile(3, 'd'),
			Square::fromRankAndFile(6, 'g'));
}

TEST(MovesOnBoardTest, RookNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1"),
			{PieceType::Rook, Color::White},
			Square::fromRankAndFile(3, 'c'),
			Square::fromRankAndFile(4, 'c'));
}

TEST(MovesOnBoardTest, RookCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1"),
			{PieceType::Rook, Color::White},
			Square::fromRankAndFile(3, 'c'),
			Square::fromRankAndFile(5, 'c'));
}

TEST(MovesOnBoardTest, QueenNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1"),
			{PieceType::Queen, Color::Black},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(2, 'f'));
}

TEST(MovesOnBoardTest, QueenCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1"),
			{PieceType::Queen, Color::Black},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'c'));
}

TEST(MovesOnBoardTest, KingNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1"),
			{PieceType::King, Color::Black},
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(8, 'b'));
}

TEST(MovesOnBoardTest, KingCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1"),
			{PieceType::King, Color::Black},
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(7, 'b'));
}

TEST(MovesOnBoardTest, KingsideCastlingWhite) {
	castlingTest(
			GameManager().createGameFromFen(
				"r2qkbnr/ppp2ppp/2np4/1B2p3/6b1/4PN2/PPPP1PPP/RNBQK2R w KQkq - 0 1"),
			Color::White,
			Square::fromRankAndFile(1, 'e'),
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(1, 'h'),
			Square::fromRankAndFile(1, 'f'));
}

TEST(MovesOnBoardTest, QueensideCastlingWhite) {
	castlingTest(
			GameManager().createGameFromFen(
				"r2qkbnr/ppp2ppp/2np4/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR w KQkq - 0 1"),
			Color::White,
			Square::fromRankAndFile(1, 'e'),
			Square::fromRankAndFile(1, 'c'),
			Square::fromRankAndFile(1, 'a'),
			Square::fromRankAndFile(1, 'd'));
}

TEST(MovesOnBoardTest, KingsideCastlingBlack) {
	castlingTest(
			GameManager().createGameFromFen(
				"rn1qk2r/ppp2ppp/3p1n2/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			Color::Black,
			Square::fromRankAndFile(8, 'e'),
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(8, 'h'),
			Square::fromRankAndFile(8, 'f'));
}

TEST(MovesOnBoardTest, QueensideCastlingBlack) {
	castlingTest(
			GameManager().createGameFromFen(
				"r3kbnr/ppp2ppp/2np4/4p1q1/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			Color::Black,
			Square::fromRankAndFile(8, 'e'),
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(8, 'a'),
			Square::fromRankAndFile(8, 'd'));
}
