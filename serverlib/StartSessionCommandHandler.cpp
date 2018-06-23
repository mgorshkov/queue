#include "StartSessionCommandHandler.h"

StartSessionCommandHandler::StartSessionCommandHandler(IQueueManager*)
{
}

MessagePtr StartSessionCommandHandler::Handle(const CompleteCommand&)
{
    return std::make_shared<StartQueueSessionMessage>();
}

