#include "Consumer.h"

Consumer::Consumer(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

Item Consumer::Consume(const std::string& aQueueName, std::size_t aOffset)
{
    return mQueueManager->Dequeue(aQueueName, aOffset);
}

