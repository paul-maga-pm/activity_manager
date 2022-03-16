#pragma once

#include <string>

class Exception
{
public:
	/// Constructor used for the base class
	/// Pre: message - const string reference
	/// Post: the message field will be initialized with the given value
	Exception(const std::string& message);

	// Returns the message of the raised exception
	const std::string& getExceptionMessage() const noexcept;

private:
	std::string member_exceptionMessage;
};

