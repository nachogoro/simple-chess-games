#include <simplechess/Exceptions.h>

using namespace simplechess;

IllegalStateException::IllegalStateException(const std::string& msg)
	: std::runtime_error(msg)
{
}

InvalidArgumentException::InvalidArgumentException(const std::string& msg)
	: std::invalid_argument(msg)
{
}
