#include "Queue.h"

void Queue::Enqueue(const ItemPtr& aItem)
{
    std::lock_guard<std::mutex> lock(mQueueMutex);
    mQueue.push(aItem);
}

ItemPtr Queue::Dequeue()
{
    std::lock_guard<std::mutex> lock(mQueueMutex);
    auto item = mQueue.front();
    mQueue.pop();
    return item;
}

