#pragma once

#include <string>

#include "Message.h"

struct CommandContext
{
    explicit CommandContext(StartQueueSessionMessagePtr aMessage);

    std::string mQueueName;
    std::size_t mOffset;
};

using CommandContextPtr = std::shared_ptr<CommandContext>;

