#include "CompleteCommand.h"

CompleteCommand::CompleteCommand(const MessagePtr& aMessage, IContextHandler* aContextHandler)
    : mContextHandler(aContextHandler)
    , mIncrementOffset(false)
{
    auto startSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(aMessage);
    if (startSessionMessage)
    {
        mCommand = Command::StartSession;
        aContextHandler->SetContext(std::make_shared<CommandContext>(startSessionMessage));
    }
    else
    {
        auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(aMessage);
        if (queueListMessage)
        {
            mCommand = Command::QueueList;
        }
        else
        {
            auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(aMessage);
            if (enqueueMessage)
            {
                mCommand = Command::Enqueue;
                mData = enqueueMessage->mData;
            }
            else
            {
                auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(aMessage);
                if (dequeueMessage)
                {
                    mCommand = Command::Dequeue;
                    mItem = dequeueMessage->mItem;
                    mIncrementOffset = true;
                }
            }
        }
    }

    mQueueName = aContextHandler->GetQueueName();
    mOffset = aContextHandler->GetOffset();
}

CompleteCommand::~CompleteCommand()
{
    if (mIncrementOffset)
        mContextHandler->IncrementOffset();
}

