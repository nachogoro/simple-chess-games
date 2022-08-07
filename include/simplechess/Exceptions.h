#ifndef EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7
#define EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7

#include <string>
#include <stdexcept>

namespace simplechess
{
	/**
	 * \brief Exceptions thrown by SimpleChess.
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
			IllegalStateException(const std::string& msg);
	};
}

#endif
