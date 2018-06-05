#include <iostream>

#include "DequeueCommandHandler.h"

DequeueCommandHandler::DequeueCommandHandler(IQueueManager* aQueueManager)
    : CommandHandler(aQueueManager)
{
}

MessagePtr DequeueCommandHandler::Handle(const CompleteCommand& aCommand)
{
    auto item = mQueueManager->Dequeue(aCommand.mQueueName, aCommand.mOffset);
    return std::make_shared<DequeueMessage>(item);
}

