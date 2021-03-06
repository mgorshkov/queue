#pragma once

#include <unordered_map>
#include <map>
#include <memory>

#include "IQueueManager.h"
#include "Defines.h"
#include "CommandHandler.h"

class CommandExecutor
{
public:
    CommandExecutor(IQueueManager* aQueueManager);

    MessagePtr RunCommand(const CompleteCommand& aCommand);

private:
    void RegisterHandler(Command aCommand, std::unique_ptr<CommandHandler> aCommandHandler);

    std::unordered_map<Command, std::unique_ptr<CommandHandler>> mCommandHandlers;
};

using CommandExecutorPtr = std::shared_ptr<CommandExecutor>;

