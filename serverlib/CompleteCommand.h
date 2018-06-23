#pragma once

#include <string>
#include "Defines.h"
#include "Message.h"
#include "IContextHandler.h"

struct CompleteCommand
{
    CompleteCommand(const MessagePtr& aMessage, IContextHandler* aContextHandler);
    ~CompleteCommand();

    Command mCommand;
    std::string mQueueName;
    std::size_t mOffset;
    DataType mData;
    Item mItem;
    IContextHandler* mContextHandler;
    bool mIncrementOffset;
};
