#include "Producer.h"

Producer::Producer(Queue& aQueue)
    : mQueue(aQueue)
{
}

void Producer::Produce(ItemPtr aItem)
{
    mQueue.Enqueue(aItem);
}

