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
        if (boost::filesystem::is_regular_file(it->status()))
            LoadQueue(it->path().filename());
    }
}

void QueueManager::LoadQueue(const boost::filesystem::path& aFileName)
{
    mQueues[aFileName.string()].Load(aFileName);
}

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

Item QueueManager::Dequeue(const std::string& aQueueName, std::size_t aOffset)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);

    return mQueues[aQueueName].Dequeue(aOffset);
}
