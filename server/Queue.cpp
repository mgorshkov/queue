#include <sstream>

#include "Queue.h"

Queue::Queue()
{
}

void Queue::Load(const boost::filesystem::path& aStorageFolderName)
{
    if (!boost::filesystem::exists(aStorageFolderName))
        boost::filesystem::create_directory(aStorageFolderName);

    for (boost::filesystem::directory_iterator it(aStorageFolderName); it != boost::filesystem::directory_iterator(); ++it)
    {
        if (!boost::filesystem::is_regular_file(it->status()))
            continue;

        if (it->path().extension() != "index")
            continue;

        auto fileName = it->path().filename().string();

        std::stringstream offsetStr(fileName);
        uintmax_t offset;
        offsetStr >> offset;

        mQueueStorage[offset] = std::make_unique<QueueStorage>(aStorageFolderName / fileName);
    }
}

void Queue::CreateStorageIfEmpty()
{
    if (mQueueStorage.empty())
        CreateStorageByOffset(0);
}

void Queue::CreateStorageByOffset(std::size_t aOffset)
{
    std::stringstream offsetStr;
    uintmax_t offset = aOffset;
    offsetStr << offset;

    mQueueStorage[offset] = std::make_unique<QueueStorage>(offsetStr.str());
}

void Queue::Enqueue(const DataType& aData)
{
    CreateStorageIfEmpty();

    auto it = std::end(mQueueStorage);
    std::advance(it, - 1);
    it->second->AddItem(aData);
}

Item Queue::Dequeue(std::size_t aOffset)
{
    auto it = mQueueStorage.lower_bound(aOffset);

    if (it != mQueueStorage.end())
        return it->second->GetItem(aOffset);
}
