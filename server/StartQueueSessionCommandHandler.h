#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class StartQueueSessionCommandHandler : public CommandHandler
{
public:
    StartQueueSessionCommandHandler(IQueueManager* aQueueManager);

    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};
