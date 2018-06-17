#include "Producer.h"

Producer::Producer(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

bool Producer::Produce(const std::string& aQueueName, const DataType& aData)
{
    return mQueueManager->Enqueue(aQueueName, aData);
}

