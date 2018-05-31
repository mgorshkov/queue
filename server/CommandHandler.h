#pragma once

#include "IQueueManager.h"
#include "CompleteCommand.h"

class CommandHandler
{
public:
    CommandHandler(IQueueManager *aQueueManager);

    virtual ~CommandHandler() = default;

    virtual CompleteOperationStatusPtr Handle(const CompleteCommand& aCommand) = 0;

protected:
    IQueueManager* mQueueManager;
};
