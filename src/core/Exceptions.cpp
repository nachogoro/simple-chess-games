#include <cpp/simplechess/Exceptions.h>

using namespace simplechess;

IllegalStateException::IllegalStateException(const std::string& msg)
	: std::runtime_error(msg)
{
}
