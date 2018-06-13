#include <iostream>

#include "Queue.h"

Queue::Queue()
    : mOffset(0)
{
}

void Queue::CreateStorage(const boost::filesystem::path& aStorageFolderName)
{
    mQueueStorage = std::make_unique<QueueStorage>(aStorageFolderName, this);
}

void Queue::Load(const boost::filesystem::path& aStorageFolderName)
{
    CreateStorage(aStorageFolderName);

    mOffsets = mQueueStorage.GetAllOffsets();
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
