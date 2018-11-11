#ifndef EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7
#define EXCEPTION_H_797BE592_5F71_439B_A718_D0FDD651FED7

#include <string>

namespace simplechess
{
	/**
	 * \brief Exception thrown by SimpleChess.
	 */
	class Exception
	{
		public:
			/**
			 * \brief Constructor.
			 *
			 * Instantiates an \c Exception with the given \c msg.
			 *
			 * \param msg The message for the \c Exception.
			 */
			Exception(const std::string& msg);

			/**
			 * \brief Returns the exception message.
			 * \return The \c Exception message.
			 */
			const std::string& what() const;
		private:
			std::string mMessage;

	};
}

#endif
