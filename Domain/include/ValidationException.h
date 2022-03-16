#pragma once

#include "Exception.h"

class ValidationException : public Exception
{
public:
	ValidationException(const std::string& message) : Exception(message) {};
};

