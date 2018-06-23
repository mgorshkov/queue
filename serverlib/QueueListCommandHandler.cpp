#include <iostream>

#include "QueueListCommandHandler.h"

QueueListCommandHandler::QueueListCommandHandler(IQueueManager* aQueueManager)
    : mQueueManager(aQueueManager)
{
}

MessagePtr QueueListCommandHandler::Handle(const CompleteCommand&)
{
#ifdef DEBUG_PRINT
    std::cout << "QueueListCommandHandler::Handle" << std::endl;
#endif
    QueueList queueList = mQueueManager->GetQueueList();
    return std::make_shared<QueueListMessage>(queueList);
}

