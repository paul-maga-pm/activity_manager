#pragma once
#include "Exception.h"
class IoFileException :
    public Exception
{
public:
    IoFileException(const std::string& message) :Exception{ message } {}
};

