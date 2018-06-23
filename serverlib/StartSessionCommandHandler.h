#pragma once

#include "CommandHandler.h"
#include "IQueueManager.h"

class StartSessionCommandHandler : public CommandHandler
{
public:
    StartSessionCommandHandler(IQueueManager*);

    virtual MessagePtr Handle(const CompleteCommand&) override;

};
