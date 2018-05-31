#include "Consumer.h"

Consumer::Consumer()
{
}

ItemPtr Consumer::Consume()
{
    mQueue.Dequeue();
}

