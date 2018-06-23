#pragma once

#include "CompleteCommand.h"

class CommandHandler
{
public:
    virtual ~CommandHandler() = default;

    virtual MessagePtr Handle(CompleteCommand& aCommand) = 0;
};
