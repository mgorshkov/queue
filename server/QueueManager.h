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

    void Enqueue(const std::string& aQueueName, const DataType& aData);
    Item Dequeue(const std::string& aQueueName, std::size_t aOffset = 0);

private:
    void LoadQueues();
    void LoadQueue(const std::string& aQueueName);
    void NewQueue(const std::string& aQueueName);
    void StartQueue(const std::string& aQueueName);

    std::mutex mQueueMutex;

    std::unordered_map<std::string, Queue> mQueues;

    constexpr static const char* QueueStorageFolder = "storage";
};
