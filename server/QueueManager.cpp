#include "QueueManager.h"

QueueList QueueManager::GetQueueList()
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    QueueList queueList;
    for (const auto& key : mQueues)
        queueList.push_back(key.first);
    return queueList;
}

void QueueManager::Enqueue(const std::string& aQueueName, const DataType& aData)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    mQueues[aQueueName].Enqueue(aData);
}

ItemPtr QueueManager::Dequeue(const std::string& aQueueName, std::size_t aOffset)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    return mQueues[aQueueName].Dequeue(aOffset);
}
