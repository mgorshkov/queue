#include "Consumer.h"

Consumer::Consumer(Queue& aQueue)
    : mQueue(aQueue)
{
}

ItemPtr Consumer::Consume()
{
    return mQueue.Dequeue();
}

