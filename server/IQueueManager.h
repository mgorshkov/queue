#pragma once

#include <string>
#include "Defines.h"

class IQueueManager
{
public:
    virtual ~IQueueManager() = default;

    virtual QueueList GetQueueList() = 0;

    virtual void Enqueue(const std::string& aQueueName, const DataType& aData) = 0;
    virtual Item Dequeue(const std::string& aQueueName, std::size_t aOffset = 0) = 0;
};
