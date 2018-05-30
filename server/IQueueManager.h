#pragma once

#include <string>
#include "Defines.h"

class IQueueManager
{
public:
    virtual ~IQueueManager() = default;

    virtual QueueList GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset = 0) = 0;

    virtual void Enqueue(const Item& aItem) = 0;
    virtual DataType Dequeue() = 0;
};
