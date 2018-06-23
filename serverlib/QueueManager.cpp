#include "QueueManager.h"

QueueManager::QueueManager()
{
    if (!boost::filesystem::exists(QueueStorageFolder))
        boost::filesystem::create_directory(QueueStorageFolder);
    else
        LoadQueues();
}

void QueueManager::LoadQueues()
{
    for (boost::filesystem::directory_iterator it(QueueStorageFolder); it != boost::filesystem::directory_iterator(); ++it)
    {
        if (boost::filesystem::is_directory(it->path()))
        {
            auto queueName = it->path().filename().string();
            LoadQueue(queueName);
        }
    }
}

std::pair<QueueManager::Queues::iterator, bool> QueueManager::LoadQueue(const std::string& aQueueName)
{
    return mQueues.insert(std::make_pair(aQueueName, boost::filesystem::path(QueueStorageFolder) / aQueueName));
}

QueueList QueueManager::GetQueueList()
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    QueueList queueList;
    for (const auto& key : mQueues)
        queueList.push_back(key.first);
    return queueList;
}

bool QueueManager::Enqueue(const std::string& aQueueName, const DataType& aData)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    auto it = mQueues.find(aQueueName);
    if (it == mQueues.end())
    {
        auto insertResult = LoadQueue(aQueueName);
        if (!insertResult.second)
            return false;
        it = insertResult.first;
    }
    it->second.Enqueue(aData);
    return true;
}

Item QueueManager::Dequeue(const std::string& aQueueName, std::size_t aOffset)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    auto it = mQueues.find(aQueueName);
    return it == mQueues.end() ?
        Item{"", static_cast<std::size_t>(-1)} : it->second.Dequeue(aOffset);
}
