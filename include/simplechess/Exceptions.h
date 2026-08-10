#ifndef EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7
#define EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7

#include <string>
#include <stdexcept>

namespace simplechess
{
	/**
	 * \brief Thrown when an operation is asked of a game which cannot
	 * perform it, such as moving in a game which has already finished.
	 *
	 * Only the operations which advance a game throw this; the methods which
	 * merely report on one never do.
	 */
	class IllegalStateException : public std::runtime_error
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * Instantiates an \c IllegalStateException with the given \c msg.
			 *
			 * \param msg The message for the \c Exception.
			 */
			explicit IllegalStateException(const std::string& msg);
	};

	/**
	 * \brief Thrown when a value handed to the library does not describe
	 * anything it can work with, such as a malformed FEN string or a square
	 * outside the board.
	 *
	 * It derives from \c std::invalid_argument, which is what these errors
	 * used to be reported as, so that existing handlers keep catching them
	 * while callers who want to tell the library's own argument errors apart
	 * from everyone else's now can.
	 */
	class InvalidArgumentException : public std::invalid_argument
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * Instantiates an \c InvalidArgumentException with the given \c
			 * msg.
			 *
			 * \param msg The message for the \c Exception.
			 */
			explicit InvalidArgumentException(const std::string& msg);
	};
}

#endif
