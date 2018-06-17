#include <iostream>

#include "QueueListCommandHandler.h"

QueueListCommandHandler::QueueListCommandHandler(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

MessagePtr QueueListCommandHandler::Handle(const CompleteCommand&)
{
    QueueList queueList = mQueueManager->GetQueueList();
    return std::make_shared<QueueListMessage>(queueList);
}

