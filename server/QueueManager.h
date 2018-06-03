#pragma once

#include <mutex>
#include <unordered_map>

#include "IQueueManager.h"
#include "Queue.h"

class QueueManager : public IQueueManager
{
public:
    QueueList GetQueueList();

    void Enqueue(const std::string& aQueueName, const DataType& aData);
    ItemPtr Dequeue(const std::string& aQueueName, std::size_t aOffset = 0);

private:
    std::mutex mQueueMutex;

    std::unordered_map<std::string, Queue> mQueues;
};
