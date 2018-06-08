#include <iostream>

#include "Queue.h"

Queue::Queue()
    : mOffset(0)
{
}

void Queue::CreateStorage(const boost::filesystem::path& aStorageFileName)
{
    mQueueStorage = std::make_unique<QueueStorage>(aStorageFileName, this);
}

void Queue::Load(const boost::filesystem::path& aStorageFileName)
{
    bool shrink = false;
    {
        std::ifstream stream(aStorageFileName.string(), std::ifstream::binary);
        Item item;
        while (stream >> item)
        {
            try
            {
                mQueue.push_back(item);
                mOffset = item.mOffset + 1;
            }
            catch(const std::bad_alloc&)
            {
                std::cout << "Memory exhausted, shrinking storage" << std::endl;
                shrink = true;
                break;
            }
        }
    }
    CreateStorage(aStorageFileName);
    if (shrink)
        Shrink();
}

void Queue::Start()
{
    mQueueStorage->Start();
}

void Queue::Enqueue(const DataType& aData)
{
    Item item(aData, mOffset);
    try
    {
        mQueue.push_back(item);
        mQueueStorage->AddItem(item);
        mOffset = item.mOffset + 1;
    }
    catch(const std::bad_alloc&)
    {
        std::cout << "Memory exhausted, shrinking storage" << std::endl;
        Shrink();
    }
}

Item Queue::Dequeue(std::size_t aOffset)
{
    std::size_t size = mQueue.size();
    if (size - mOffset + aOffset < 0)
        return Item{"", static_cast<std::size_t>(-1)};
    return mQueue[size - mOffset + aOffset];
}

void Queue::Shrink()
{
    // remove oldest quarter of the queue
    std::size_t size = mQueue.size();
    mQueue.erase(mQueue.begin(), mQueue.begin() + size / 4);
    mQueueStorage->ScheduleShrinkStorage();
}
