#ifndef ALGEBRAIC_NOTATION_GENERATOR_H_D2B88175_AF8F_49B0_99A5_05630DE54230
#define ALGEBRAIC_NOTATION_GENERATOR_H_D2B88175_AF8F_49B0_99A5_05630DE54230

#include <cpp/simplechess/Board.h>
#include <cpp/simplechess/PieceMove.h>
#include <cpp/simplechess/PlayedMove.h>

#include <string>

namespace simplechess
{
	namespace details
	{
		class AlgebraicNotationGenerator
		{
			public:
				static std::string toAlgebraicNotation(
						const Board& board,
						const PieceMove& move,
						const bool drawOffered,
						const CheckType checkType);
		};
	}
}

#endif
