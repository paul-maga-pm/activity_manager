#pragma once
#include "Exception.h"
class RepositoryException : public Exception
{
public:
	RepositoryException(const std::string& message) : Exception(message) {};
};

