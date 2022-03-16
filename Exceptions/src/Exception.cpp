#include "Exception.h"

Exception::Exception(const std::string& message) :member_exceptionMessage{ message } {}

const std::string& Exception::getExceptionMessage() const noexcept
{
	return member_exceptionMessage;
}