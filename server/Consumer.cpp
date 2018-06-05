#include "Consumer.h"

Consumer::Consumer(Queue& aQueue)
    : mQueue(aQueue)
{
}

Item Consumer::Consume(std::size_t aOffset)
{
    return mQueue.Dequeue(aOffset);
}

