#include "AlgebraicNotationParser.h"

#include <regex>
#include <string>

using namespace simplechess;

Square AlgebraicNotationParser::parseSquare(const std::string& str) const
{
	if (str.size() != 2)
	{
		throw Exception("Invalid square: " + str);
	}

	const uint8_t rank = str[1];
	const char file = tolower(str[0]);

	if (rank < 1 || rank > 8 || file < 'a' || file > 'h')
	{
		throw Exception("Invalid square: " + str);
	}

	return Square(rank, file);
}

Move AlgebraicNotationParser::parseMove(
		const Board& board, Color toMove, const std::string& str) const
{
	// Notation rules:
	// 1. Moves are indicated by a letter representing the piece that is moving
	//    followed by the square it is moving to (e.g. Be5 means moving the
	//    bishop to e5)
	// 2. A pawn does not have a leading letter (e.g. e5 means moving the pawn
	//    to e5).
	// 3. If a capture happens, an 'x' is placed between the piece and the
	//    final square (e.g. Bxe5).
	// 4. In the case of a pawn capturing, the file from which the pawn departs
	//    is used (e.g. fxe5)
	// 5. "En passant" captures are the same as regular pawn captures but may
	//    include the prefix "e.p." (e.g. exd6e.p.)
	// 6. For disambiguation between two identical pieces which can move to the
	//    same square, the moving piece is identified by its letter followed by
	//    (in order of preference):
	//      - The file of departure (if they differ, e.g. Ngf3);
	//      - The rank of departure (if files are equal but rank differ, e.g.
	//        N5f3);
	//      - Both file and rank of departure (if neither alone is enough to
	//        identify a piece unambiguously, e.g. Qa1b2)
	// 7. When a pawn is promoted, the piece to which it promotes is indicated
	//    at the end of the move (e.g. e8Q)
	// 8. Castling is indicated with "0-0" for kingside castling and "0-0-0"
	//    for queenside castling.
	// 9. A check is indicated with a "+" at the end of the notation (e.g.
	//    exd6+)
	// 10. Check mate is indicated by "#" at the end of the notation (e.g.
	//     exd6#)
	// 11. End of game is indicated as "1-0" for whites, "0-1" for blacks or
	//     "½-½" for a draw.
	// 12. A draw offer is represented by appending "(=)" at the end of the
	//     notation (e.g. Bxe6 (=))

	// TODO: address draw offers
	// Regular move: [RNBQ][a-h]{0,1}[1-8]{0,1}[x]{0,1}[a-h][1-8][+#]{0,1}
	// Pawn move (no promotion): ([a-h]x){0,1}[a-h][2-7](e\.p\.){0,1}[+#]{0,1}
	// Pawn promotion (promotion): ([a-h]x){0,1}[a-h][18][RNBQ][+#]{0,1}

	std::regex regularMove("[RNBQ][a-h]{0,1}[1-8]{0,1}[x]{0,1}[a-h][1-8]((\+{0,1}( \(=\)){0,1})|#{0,1})");
	std::regex pawnMoveNoPromotion("([a-h]x){0,1}[a-h][2-7](e\.p\.){0,1}((\+{0,1}( \(=\)){0,1})|#{0,1})");
	std::regex pawnMovePromotion("([a-h]x){0,1}[a-h][18][RNBQ]((\+{0,1}( \(=\)){0,1})|#{0,1})");
	std::regex castlingKingSide("0-0((\+{0,1}( \(=\)){0,1})|#{0,1})");
	std::regex castlingQueenSide("0-0-0((\+{0,1}( \(=\)){0,1})|#{0,1})");
	std::regex whiteWins("1-0");
	std::regex blackWins("0-1");
	std::regex draw("½-½");

	std::smatch regex_match;
	if (std::regex_match(str, regex_match, regularMove))
	{
		// Process regular move
	}
	else if (std::regex_match(str, regex_match, pawnMoveNoPromotion))
	{
		// Process pawn move
	}
	else if (std::regex_match(str, regex_match, pawnMovePromotion))
	{
		// Process pawn promotion
	}
	else if (std::regex_match(str, regex_match, castlingKingSide))
	{
		// Process castling king side
	}
	else if (std::regex_match(str, regex_match, castlingQueenSide))
	{
		// Process castling queen side
	}
	else if (std::regex_match(str, regex_match, whiteWins)
				|| std::regex_match(str, regex_match, blackWins))
	{
		// Process win
	}
	else if (std::regex_match(str, regex_match, pawnMovePromotion))
	{
		// Process pawn promotion
	}
	else
	{
		throw Exception(str + " is not a valid move in algebraic notation");
	}
}
