#include "Queue.h"

Queue::Queue()
    : mOffset(0)
{
}

void Queue::Load(const boost::filesystem::path& aFileName)
{
    mQueueStorage.Load(aFileName);
}

void Queue::Enqueue(const DataType& aData)
{
    Item item(aData, mOffset++);
    mQueueStorage.AddItem(item);
    try
    {
        mQueue.push_back(item);
    }
    catch(const std::bad_alloc& ex)
    {

    }
}

Item Queue::Dequeue(std::size_t aOffset)
{
    assert(aOffset < mQueue.size());
    return mQueue[aOffset];
}

