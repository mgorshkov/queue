#pragma once

#include <string>
#include "Defines.h"
#include "Message.h"

struct CompleteCommand
{
    CompleteCommand(const MessagePtr& aMessage);

    Command mCommand;
    std::string mQueueName;
    std::size_t mOffset;
    DataType mData;
    Item mItem;
};
