#include <iostream>

#include "EnqueueCommandHandler.h"

EnqueueCommandHandler::EnqueueCommandHandler(IQueueManager* aQueueManager)
    : mProducer(aQueueManager)
{
}

MessagePtr EnqueueCommandHandler::Handle(CompleteCommand& aCommand)
{
    mProducer.Produce(aCommand.mQueueName, aCommand.mData);
    return std::make_shared<Message>();
}

