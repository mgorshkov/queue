#pragma once

#include <string>
#include "Defines.h"
#include "Message.h"
#include "CommandContext.h"

struct CompleteCommand
{
    CompleteCommand(const MessagePtr& aMessage, CommandContextPtr aContext);

    Command mCommand;
    std::string mQueueName;
    std::size_t mOffset;
    DataType mData;
    Item mItem;
};
