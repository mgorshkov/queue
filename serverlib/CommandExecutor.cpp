#include <assert.h>
#include <iostream>

#include "CommandExecutor.h"
#include "DequeueCommandHandler.h"
#include "EnqueueCommandHandler.h"
#include "QueueListCommandHandler.h"
#include "StartSessionCommandHandler.h"

CommandExecutor::CommandExecutor(IQueueManager* aQueueManager)
{
    RegisterHandler(Command::StartSession, std::make_unique<StartSessionCommandHandler>(aQueueManager));
    RegisterHandler(Command::Dequeue, std::make_unique<DequeueCommandHandler>(aQueueManager));
    RegisterHandler(Command::Enqueue, std::make_unique<EnqueueCommandHandler>(aQueueManager));
    RegisterHandler(Command::QueueList, std::make_unique<QueueListCommandHandler>(aQueueManager));
}

MessagePtr CommandExecutor::RunCommand(const CompleteCommand& aCommand)
{
    assert (mCommandHandlers.find(aCommand.mCommand) != mCommandHandlers.end());
    return mCommandHandlers[aCommand.mCommand]->Handle(aCommand);
}

void CommandExecutor::RegisterHandler(Command aCommand, std::unique_ptr<CommandHandler> aCommandHandler)
{
    assert (mCommandHandlers.find(aCommand) == mCommandHandlers.end());
    mCommandHandlers.emplace(std::make_pair(aCommand, std::move(aCommandHandler)));
}
