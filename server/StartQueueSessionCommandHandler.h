#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class StartQueueSessionCommandHandler : public CommandHandler
{
public:
    StartQueueSessionCommandHandler(IQueueManager* aQueueManager);

    virtual CompleteOperationStatusPtr Handle(const CompleteCommand& aCommand) override;
};
