#include <sstream>
#include <iomanip>

#include "Queue.h"

Queue::Queue(const boost::filesystem::path& aStorageFolderName)
    : mStorageFolderName(aStorageFolderName)
{
    Load();
}

void Queue::Load()
{
#ifdef DEBUG_PRINT
    std::cout << "Queue::Load aStorageFolderName=" << mStorageFolderName << std::endl;
#endif
    if (!boost::filesystem::exists(mStorageFolderName))
        boost::filesystem::create_directory(mStorageFolderName);

    for (boost::filesystem::directory_iterator it(mStorageFolderName); it != boost::filesystem::directory_iterator(); ++it)
    {
        if (!boost::filesystem::is_regular_file(it->status()))
            continue;

        if (it->path().extension() != "index")
            continue;

        auto fileName = it->path().filename().string();

        std::stringstream offsetStr(fileName);
        uintmax_t offset;
        offsetStr >> offset;

        mQueueStorage[offset] = std::make_unique<QueueStorage>(it->path());
    }
}

void Queue::CreateStorageIfEmpty()
{
    if (mQueueStorage.empty())
        CreateStorageByOffset(0);
}

void Queue::CreateStorageByOffset(std::size_t aOffset)
{
#ifdef DEBUG_PRINT
    std::cout << "Queue::CreateStorageByOffset, aOffset=" << aOffset << std::endl;
#endif
    std::stringstream offsetStr;
    offsetStr << std::setfill('0') << std::setw(2 * sizeof(aOffset)) << std::uppercase << std::hex << aOffset;

    mQueueStorage[aOffset] = std::make_unique<QueueStorage>(mStorageFolderName / offsetStr.str());
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
