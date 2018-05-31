#include <iostream>

#include "DequeueCommandHandler.h"

DequeueCommandHandler::DequeueCommandHandler(IQueueManager* aQueueManager)
    : CommandHandler(aQueueManager)
{
}

CompleteOperationStatusPtr DequeueCommandHandler::Handle(const CompleteCommand& aCommand)
{
    Item item = mQueueManager->Dequeue(aCommand.mQueueName, aCommand.mOffset);
    return std::make_shared<ItemOperationStatus>(item);
}

