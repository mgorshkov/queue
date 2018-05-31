#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class QueueListCommandHandler : public CommandHandler
{
public:
    QueueListCommandHandler(IQueueManager* aQueueManager);

    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};
