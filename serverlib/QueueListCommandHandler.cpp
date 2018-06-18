#include <iostream>

#include "QueueListCommandHandler.h"

QueueListCommandHandler::QueueListCommandHandler(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

MessagePtr QueueListCommandHandler::Handle(const CompleteCommand&)
{
std::cout << "QueueListCommandHandler::Handle" << std::endl;
    QueueList queueList = mQueueManager->GetQueueList();
    return std::make_shared<QueueListMessage>(queueList);
}

