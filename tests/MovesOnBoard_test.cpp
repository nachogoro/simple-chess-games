#include <gtest/gtest.h>
#include "GameManager.h"

#include <boost/optional/optional_io.hpp>

using namespace simplechess;

namespace
{
	enum Difference
	{
		PIECE_APPEARED,
		PIECE_DISAPPEARED,
		PIECE_REPLACED
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
			const boost::optional<Piece> inBefore = before.pieceAt(entry.first);
			if (!inBefore)
			{
				// This square was empty in before but not in after, a piece
				// appeared
				result.insert({entry.first, {entry.second, PIECE_APPEARED}});
			}
			else if (*inBefore != entry.second)
			{
				// The contents of this square have changed
				result.insert({entry.first, {entry.second, PIECE_REPLACED}});
			}
		}

		for (const auto& entry : before.occupiedSquares())
		{
			const boost::optional<Piece> inAfter = after.pieceAt(entry.first);
			if (!inAfter)
			{
				// This square was empty in before but not in after, a piece
				// appeared
				result.insert({entry.first, {entry.second, PIECE_DISAPPEARED}});
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
				src, {piece, PIECE_DISAPPEARED}
			},

			{
				dst, {piece, PIECE_APPEARED}
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
				src, {piece, PIECE_DISAPPEARED}
			},

			{
				dst, {piece, PIECE_REPLACED}
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
		const Piece king = {TYPE_KING, castlingColor};
		const Piece rook = {TYPE_ROOK, castlingColor};

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
				kingSrc, {king, PIECE_DISAPPEARED}
			},

			{
				kingDst, {king, PIECE_APPEARED}
			},

			{
				rookSrc, {rook, PIECE_DISAPPEARED}
			},

			{
				rookDst, {rook, PIECE_APPEARED}
			},
		};

		EXPECT_EQ(comparison, expected);
	}
}

TEST(MovesOnBoardTest, PawnOnceForward) {
	regularNonCaptureTest(
			GameManager().createNewGame(),
			{TYPE_PAWN, COLOR_WHITE},
			Square::fromRankAndFile(2, 'f'),
			Square::fromRankAndFile(3, 'f'));
}

TEST(MovesOnBoardTest, PawnTwiceForward) {
	regularNonCaptureTest(
			GameManager().createNewGame(),
			{TYPE_PAWN, COLOR_WHITE},
			Square::fromRankAndFile(2, 'a'),
			Square::fromRankAndFile(4, 'a'));
}

TEST(MovesOnBoardTest, PawnCapture) {
	regularCaptureTest(
			GameManager().createGameFromFen(
				"rn1qk2r/ppp2ppp/3p1n2/4p3/3P2b1/2N1P3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			{TYPE_PAWN, COLOR_BLACK},
			Square::fromRankAndFile(5, 'e'),
			Square::fromRankAndFile(4, 'd'));
}

TEST(MovesOnBoardTest, WhitePawnEnPassant) {
	const Game startingGame = GameManager().createGameFromFen(
			"rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

	const Square src = Square::fromRankAndFile(5, 'd');
	const Square dst = Square::fromRankAndFile(6, 'e');
	const Square squareOfCapturedPawn = Square::fromRankAndFile(5, 'e');

	const Piece piece = {TYPE_PAWN, COLOR_WHITE};

	const Game afterPawnMove = GameManager().makeMove(
			startingGame,
			PieceMove::regularMove(
				{TYPE_PAWN, COLOR_WHITE},
				src,
				dst));

	const std::map<Square, Effect> comparison = ::compareBoards(
			afterPawnMove.currentStage().board(),
			startingGame.currentStage().board());

	const std::map<Square, Effect> expected =
	{
		{
			src, {piece, PIECE_DISAPPEARED}
		},

		{
			dst, {piece, PIECE_APPEARED}
		},

		{
			squareOfCapturedPawn, {{TYPE_PAWN, COLOR_BLACK}, PIECE_DISAPPEARED}
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

	const Piece piece = {TYPE_PAWN, COLOR_BLACK};

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
			src, {piece, PIECE_DISAPPEARED}
		},

		{
			dst, {piece, PIECE_APPEARED}
		},

		{
			squareOfCapturedPawn, {{TYPE_PAWN, COLOR_WHITE}, PIECE_DISAPPEARED}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, PawnPromotionNoCapture) {
	const Game startingGame = GameManager().createGameFromFen(
			"8/4k3/8/2q5/7P/2RQ4/1K4p1/8 b - - 0 1");

	const Square src = Square::fromRankAndFile(2, 'g');
	const Square dst = Square::fromRankAndFile(1, 'g');

	const Piece piece = {TYPE_PAWN, COLOR_BLACK};
	const Piece promoted = {TYPE_QUEEN, COLOR_BLACK};

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
			src, {piece, PIECE_DISAPPEARED}
		},

		{
			dst, {promoted, PIECE_APPEARED}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, PawnPromotionCapture) {
	const Game startingGame = GameManager().createGameFromFen(
			"2q5/1P2k3/8/8/8/2RQ4/1K4p1/8 w - - 0 1");

	const Square src = Square::fromRankAndFile(7, 'b');
	const Square dst = Square::fromRankAndFile(8, 'c');

	const Piece piece = {TYPE_PAWN, COLOR_WHITE};
	const Piece promoted = {TYPE_QUEEN, COLOR_WHITE};

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
			src, {piece, PIECE_DISAPPEARED}
		},

		{
			dst, {promoted, PIECE_REPLACED}
		}
	};

	EXPECT_EQ(comparison, expected);
}

TEST(MovesOnBoardTest, KnightNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1"),
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'b'));
}

TEST(MovesOnBoardTest, KnightCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/8/2n5/6pP/3B4/1K6/8 b - h3 0 1"),
			{TYPE_KNIGHT, COLOR_BLACK},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'd'));
}

TEST(MovesOnBoardTest, BishopNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1"),
			{TYPE_BISHOP, COLOR_WHITE},
			Square::fromRankAndFile(3, 'd'),
			Square::fromRankAndFile(1, 'f'));
}

TEST(MovesOnBoardTest, BishopCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/3B4/1K6/8 w - - 0 1"),
			{TYPE_BISHOP, COLOR_WHITE},
			Square::fromRankAndFile(3, 'd'),
			Square::fromRankAndFile(6, 'g'));
}

TEST(MovesOnBoardTest, RookNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1"),
			{TYPE_ROOK, COLOR_WHITE},
			Square::fromRankAndFile(3, 'c'),
			Square::fromRankAndFile(4, 'c'));
}

TEST(MovesOnBoardTest, RookCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2n5/7P/2RB4/1K6/8 w - - 0 1"),
			{TYPE_ROOK, COLOR_WHITE},
			Square::fromRankAndFile(3, 'c'),
			Square::fromRankAndFile(5, 'c'));
}

TEST(MovesOnBoardTest, QueenNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1"),
			{TYPE_QUEEN, COLOR_BLACK},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(2, 'f'));
}

TEST(MovesOnBoardTest, QueenCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"8/4k3/6p1/2q5/7P/2RQ4/1K6/8 b - - 0 1"),
			{TYPE_QUEEN, COLOR_BLACK},
			Square::fromRankAndFile(5, 'c'),
			Square::fromRankAndFile(3, 'c'));
}

TEST(MovesOnBoardTest, KingNoCapture) {
	regularNonCaptureTest(GameManager().createGameFromFen(
				"2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1"),
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(8, 'b'));
}

TEST(MovesOnBoardTest, KingCapture) {
	regularCaptureTest(GameManager().createGameFromFen(
				"2k5/1P6/8/8/8/2RQ4/1K4p1/8 b - - 0 1"),
			{TYPE_KING, COLOR_BLACK},
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(7, 'b'));
}

TEST(MovesOnBoardTest, KingsideCastlingWhite) {
	castlingTest(
			GameManager().createGameFromFen(
				"r2qkbnr/ppp2ppp/2np4/1B2p3/6b1/4PN2/PPPP1PPP/RNBQK2R w KQkq - 0 1"),
			COLOR_WHITE,
			Square::fromRankAndFile(1, 'e'),
			Square::fromRankAndFile(1, 'g'),
			Square::fromRankAndFile(1, 'h'),
			Square::fromRankAndFile(1, 'f'));
}

TEST(MovesOnBoardTest, QueensideCastlingWhite) {
	castlingTest(
			GameManager().createGameFromFen(
				"r2qkbnr/ppp2ppp/2np4/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR w KQkq - 0 1"),
			COLOR_WHITE,
			Square::fromRankAndFile(1, 'e'),
			Square::fromRankAndFile(1, 'c'),
			Square::fromRankAndFile(1, 'a'),
			Square::fromRankAndFile(1, 'd'));
}

TEST(MovesOnBoardTest, KingsideCastlingBlack) {
	castlingTest(
			GameManager().createGameFromFen(
				"rn1qk2r/ppp2ppp/3p1n2/4p3/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			COLOR_BLACK,
			Square::fromRankAndFile(8, 'e'),
			Square::fromRankAndFile(8, 'g'),
			Square::fromRankAndFile(8, 'h'),
			Square::fromRankAndFile(8, 'f'));
}

TEST(MovesOnBoardTest, QueensideCastlingBlack) {
	castlingTest(
			GameManager().createGameFromFen(
				"r3kbnr/ppp2ppp/2np4/4p1q1/6b1/2NPP3/PPPBQPPP/R3KBNR b KQkq - 0 1"),
			COLOR_BLACK,
			Square::fromRankAndFile(8, 'e'),
			Square::fromRankAndFile(8, 'c'),
			Square::fromRankAndFile(8, 'a'),
			Square::fromRankAndFile(8, 'd'));
}
