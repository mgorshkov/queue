#pragma once

#include <mutex>
#include <unordered_map>
#include <boost/filesystem.hpp>

#include "IQueueManager.h"
#include "Queue.h"

class QueueManager : public IQueueManager
{
public:
    QueueManager();

    QueueList GetQueueList();

    bool Enqueue(const std::string& aQueueName, const DataType& aData);
    Item Dequeue(const std::string& aQueueName, std::size_t aOffset = 0);

private:
    void LoadQueues();

    using Queues = std::unordered_map<std::string, Queue>;
    std::pair<Queues::iterator, bool> LoadQueue(const std::string& aQueueName);

    std::mutex mQueueMutex;

    Queues mQueues;

    constexpr static const char* QueueStorageFolder = "storage";
};
