#ifndef ALGEBRAIC_NOTATION_PARSER_H_93EF87D1_3885_495B_A000_A6BB9E851694
#define ALGEBRAIC_NOTATION_PARSER_H_93EF87D1_3885_495B_A000_A6BB9E851694

#include <Move.h>
#include <Square.h>
#include <Exception.h>

namespace simplechess
{
	/**
	 * \brief Parser class for moves and squares in algebraic notation.
	 */
	class AlgebraicNotationParser
	{
		public:
			Square parseSquare(const std::string& str) const;
			Move parseMove(const std::string& str) const;
	};
}

#endif /* ALGEBRAIC_NOTATION_PARSER_H_93EF87D1_3885_495B_A000_A6BB9E851694 */
