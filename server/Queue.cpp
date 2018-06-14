#include <iostream>

#include "Queue.h"

Queue::Queue()
    : mOffset(0)
{
}

void Queue::CreateStorage(const boost::filesystem::path& aStorageFolderName)
{
    for (boost::filesystem::directory_iterator it(mStorageFilePath); it != boost::filesystem::directory_iterator(); ++it)
    {
        if (!boost::filesystem::is_regular_file(it->status()))
            continue;

        if (it->path().extension() != "index")
            continue;

        auto fileName = it->path().filename().string();

        std::stringstream offsetStr(fileName);
        uintmax_t offset;
        offsetStr >> offset;

        mQueueStorage[offset] = std::make_unique<QueueStorage>(fileName, this);
    }
}

void Queue::Load(const boost::filesystem::path& aStorageFolderName)
{
    CreateStorage(aStorageFolderName);

    mOffsets = mQueueStorage.GetAllOffsets();
}

void Queue::FoldStorage(std::size_t aOffset)
{
    mStream.close();

    std::stringstream offsetStr;
    offsetStr << aOffset;

    mStorageFileName = offsetStr.str();

//    mStream.open(mStorageFileName.string(), std::fstream::out | std::fstream::app);
}

void Queue::Enqueue(const DataType& aData)
{
    Item item(aData, mOffset);
    mQueueStorage->AddItem(item);
    mOffset = item.mOffset + 1;
}

Item Queue::Dequeue(std::size_t aOffset)
{
    if (aOffset >= mMinOffset && aOffset <= mMaxOffset)
        return mQueue[aOffset - mMinOffset];
    else if (aOffset < mMinOffset)
}
