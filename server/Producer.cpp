#include "Producer.h"

Producer::Producer(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

void Producer::Produce(const std::string& aQueueName, const DataType& aData)
{
    mQueueManager->Enqueue(aQueueName, aData);
}

