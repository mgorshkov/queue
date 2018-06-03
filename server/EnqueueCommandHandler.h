#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class EnqueueCommandHandler : public CommandHandler
{
public:
    EnqueueCommandHandler(IQueueManager* aQueueManager);

    virtual MessagePtr Handle(const CompleteCommand& aCommand) override;
};
