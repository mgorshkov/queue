#include "Queue.h"

void Queue::Enqueue(const Item& aItem)
{
    std::lock_quard<std::mutex> lock(mQueueMutex);
    mQueue.enqueue(aItem);
}

ItemPtr Queue::Dequeue()
{
    std::lock_quard<std::mutex> lock(mQueueMutex);
    auto item = mQueue.front();
    mQueue.pop();
    return item;
}

