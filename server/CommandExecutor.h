#pragma once

#include <unordered_map>
#include <map>
#include <memory>

#include "itablemanager.h"
#include "structs.h"
#include "commandhandler.h"

class CommandExecutor
{
public:
    CommandExecutor(ITableManager* aTableManager);

    CompleteOperationStatus RunCommand(const std::string& aLine);

private:
    void RegisterHandler(Command aCommand, std::unique_ptr<CommandHandler> aCommandHandler);
    CompleteCommand Parse(const std::string& aLine);

    std::unordered_map<Command, std::unique_ptr<CommandHandler>> mCommandHandlers;
};
