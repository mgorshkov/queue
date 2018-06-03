#include <iostream>

#include "EnqueueCommandHandler.h"

EnqueueCommandHandler::EnqueueCommandHandler(IQueueManager* aQueueManager)
    : CommandHandler(aQueueManager)
{
}

MessagePtr EnqueueCommandHandler::Handle(const CompleteCommand& aCommand)
{
    mQueueManager->Enqueue(aCommand.mQueueName, aCommand.mData);
    return std::make_shared<Message>();
}

