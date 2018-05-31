#pragma once

#include "IQueueManager.h"

class CommandHandler
{
public:
    CommandHandler(IQueueManager *aQueueManager);

    virtual ~CommandHandler() = default;

    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) = 0;

protected:
    IQueueManager* mQueueManager;
};
