#include "Queue.h"

Queue::Queue()
    : mOffset(0)
{
}

void Queue::Load(const std::string& aFileName)
{
    mQueueStorage.Load(aFIleName);
}

void Queue::Enqueue(const DataType& aData)
{
    ItemPtr item = std::make_shared<Item>(aData, mOffset++);
    mQueueStorage.Load(aFIleName);
    try
    {
        mQueue.push_back(item);
}

ItemPtr Queue::Dequeue(std::size_t aOffset)
{
    assert(aOffset < mQueue.size());
    return mQueue[aOffset];
}

