#include <sstream>

#include "Queue.h"

Queue::Queue()
    : mCurrentOffset(0)
{
}

void Queue::Load(const boost::filesystem::path& aStorageFolderName)
{
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

        mQueueStorage[offset] = std::make_unique<QueueStorage>(fileName);
    }
}

void Queue::Enqueue(const DataType& aData)
{
    Item item(aData, mCurrentOffset);
    if (mQueueStorage.empty())
    {
        std::stringstream offsetStr;
        uintmax_t offset = 0;
        offsetStr << offset;

        mQueueStorage[offset] = std::make_unique<QueueStorage>(offsetStr.str());
    }

    auto it = std::end(mQueueStorage);
    std::advance(it, - 1);
    it->second->AddItem(item);
    mCurrentOffset = item.mOffset + 1;
}

Item Queue::Dequeue(std::size_t aOffset)
{
    auto it = mQueueStorage.lower_bound(aOffset);

    if (it != mQueueStorage.end())
        return it->second->GetItem(aOffset);
}
