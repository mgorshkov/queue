#include "StartSessionCommandHandler.h"

StartSessionCommandHandler::StartSessionCommandHandler(IQueueManager*)
{
}

MessagePtr StartSessionCommandHandler::Handle(CompleteCommand&)
{
    return nullptr;
}

