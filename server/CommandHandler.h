#pragma once

#include "CompleteCommand.h"

class CommandHandler
{
public:
    virtual ~CommandHandler() = default;

    virtual MessagePtr Handle(const CompleteCommand& aCommand) = 0;
};
