#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"
#include "Consumer.h"

class DequeueCommandHandler : public CommandHandler
{
public:
    DequeueCommandHandler(IQueueManager* aQueueManager);

    virtual MessagePtr Handle(const CompleteCommand& aCommand) override;

private:
    Consumer mConsumer;
};
