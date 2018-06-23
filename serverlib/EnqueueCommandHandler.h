#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"
#include "Producer.h"

class EnqueueCommandHandler : public CommandHandler
{
public:
    EnqueueCommandHandler(IQueueManager* aQueueManager);

    virtual MessagePtr Handle(CompleteCommand& aCommand) override;

private:
    Producer mProducer;
};
