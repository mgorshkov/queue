#pragma once

#include <mutex>
#include <unordered_map>

#include "IQueueManager.h"
#include "Queue.h"

class QueueManager : public IQueueManager
{
public:
    QueueManager();

    QueueList GetQueueList();

    void Enqueue(const std::string& aQueueName, const DataType& aData);
    ItemPtr Dequeue(const std::string& aQueueName, std::size_t aOffset = 0);

private:
    void LoadQueues();
    void LoadQueue(const std::string& aFileName);

    std::mutex mQueueMutex;

    std::unordered_map<std::string, Queue> mQueues;

    constexpr static const char* QueueStorageFolder = "storage";
};
