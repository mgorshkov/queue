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

        if (it->path().extension() != ".index")
            continue;

        auto fileName = it->path().filename().string();

        std::stringstream offsetStr(fileName);
        uintmax_t offset;
        offsetStr >> std::hex >> offset;

        mQueueStorage[offset] = std::make_unique<QueueStorage>(it->path());
    }
}

void Queue::CreateStorageIfEmpty()
{
    if (mQueueStorage.empty())
        CreateStorageByOffset(0);
}

void Queue::DeleteFirstStorage()
{
    assert (!mQueueStorage.empty());
    auto it = std::begin(mQueueStorage);
    auto path = GetStorageFileNameByOffset(it->first);
    boost::filesystem::remove(path);
    mQueueStorage.erase(it);
}

boost::filesystem::path Queue::GetStorageFileNameByOffset(std::size_t aOffset)
{
    std::stringstream offsetStr;
    offsetStr << std::setfill('0') << std::setw(2 * sizeof(aOffset)) << std::uppercase << std::hex << aOffset;
    return mStorageFolderName / offsetStr.str();
}

void Queue::CreateStorageByOffset(std::size_t aOffset)
{
#ifdef DEBUG_PRINT
    std::cout << "Queue::CreateStorageByOffset, aOffset=" << aOffset << std::endl;
#endif
    auto fileName = GetStorageFileNameByOffset(aOffset);

    mQueueStorage[aOffset] = std::make_unique<QueueStorage>(fileName);
}

void Queue::Enqueue(const DataType& aData)
{
    CreateStorageIfEmpty();

    QueueStorage::AddStatus status = QueueStorage::AddStatus::Ok;
    do
    {
        auto it = std::prev(std::end(mQueueStorage));
        status = it->second->AddData(aData);
        switch (status)
        {
        case QueueStorage::AddStatus::Ok:
            break;

        case QueueStorage::AddStatus::FileFull:
            CreateStorageByOffset(it->second->GetNextOffset());
            break;

        case QueueStorage::AddStatus::DiskFull:
            break;

        default:
            assert(0);
        }
    }
    while (status != QueueStorage::AddStatus::Ok);
}

bool Queue::Dequeue(std::size_t aOffset, DataType& aData)
{
    auto it = std::prev(mQueueStorage.upper_bound(aOffset));

    return it->second->GetData(aOffset, aData);
}
