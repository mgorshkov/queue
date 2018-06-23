#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class QueueListCommandHandler : public CommandHandler
{
public:
    QueueListCommandHandler(IQueueManager* aQueueManager);

    virtual MessagePtr Handle(CompleteCommand& aCommand) override;

private:
    IQueueManager* mQueueManager;
};
