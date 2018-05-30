#pragma once

#include "IQueueManager.h"

class CommandHandler
{
public:
    CommandHandler(IQueueManager *aQueueManager);

    virtual ~CommandHandler() = default;

    virtual std::string GetCommand() const = 0;
    virtual CompleteCommand Parse(const std::string& aLine) = 0;
    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) = 0;

protected:
    IQueueManager* mQueueManager;
};
