#include "CompleteCommand.h"

CompleteCommand::CompleteCommand(const MessagePtr& aMessage, CommandContextPtr aContext)
{
    auto startSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(aMessage);
    if (startSessionMessage)
    {
        mCommand = Command::StartSession;
        aContext = std::make_shared<CommandContext>(startSessionMessage);
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
                }
            }
        }
    }
    if (aContext)
    {
        mQueueName = aContext->mQueueName;
        mOffset = aContext->mOffset;
    }
}

