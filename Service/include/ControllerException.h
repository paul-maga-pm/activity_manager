#pragma once
#include "Exception.h"
class ControllerException :
    public Exception
{
public:
    ControllerException(const std::string& message) : Exception{ message } {};
};

