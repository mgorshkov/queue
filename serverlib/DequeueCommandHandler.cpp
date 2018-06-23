#include <iostream>

#include "DequeueCommandHandler.h"

DequeueCommandHandler::DequeueCommandHandler(IQueueManager* aQueueManager)
    : mConsumer(aQueueManager)
{
}

MessagePtr DequeueCommandHandler::Handle(CompleteCommand& aCommand)
{
    auto item = mConsumer.Consume(aCommand.mQueueName, aCommand.mOffset++);
    return std::make_shared<DequeueMessage>(item);
}

