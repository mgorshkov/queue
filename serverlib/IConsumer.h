#pragma once

#include "Defines.h"

class IConsumer
{
public:
    virtual ~IConsumer() = default;
    virtual Item Consume(const std::string& aQueueName, std::size_t aOffset) = 0;
};
