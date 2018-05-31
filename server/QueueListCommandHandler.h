#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class QueueListCommandHandler : public CommandHandler
{
public:
    QueueListCommandHandler(IQueueManager* aQueueManager);

    virtual CompleteOperationStatusPtr Handle(const CompleteCommand& aCommand) override;
};
