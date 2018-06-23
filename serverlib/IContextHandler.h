#pragma once

#include "CommandContext.h"
#include <string>

class IContextHandler
{
public:
    virtual void SetContext(CommandContextPtr aContext) = 0;

    virtual std::string GetQueueName() const = 0;
    virtual std::size_t GetOffset() const = 0;
    virtual void IncrementOffset() = 0;
};


