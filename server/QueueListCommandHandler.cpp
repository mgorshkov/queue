#include <iostream>

#include "QueueListCommandHandler.h"

QueueListCommandHandler::QueueListCommandHandler(IQueueManager* aQueueManager)
    : CommandHandler(aQueueManager)
{		
}

CompleteOperationStatusPtr QueueListCommandHandler::Handle(const CompleteCommand&)
{
    QueueList queueList = mQueueManager->GetQueueList();
    return std::make_shared<QueueListOperationStatus>(queueList);
}

