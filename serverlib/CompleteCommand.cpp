#include "CompleteCommand.h"

CompleteCommand::CompleteCommand(const MessagePtr& aMessage, const CommandContext& aContext)
{
    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(aMessage);
    if (queueListMessage)
    {
        mCommand = Command::QueueList;
        return;
    }
    auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(aMessage);
    if (enqueueMessage)
    {
        mCommand = Command::Enqueue;
        mData = enqueueMessage->mData;
        mQueueName = aContext.mQueueName;
        mOffset = aContext.mOffset;
        return;
    }
    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(aMessage);
    if (dequeueMessage)
    {
        mCommand = Command::Dequeue;
        mItem = dequeueMessage->mItem;
        mQueueName = aContext.mQueueName;
        mOffset = aContext.mOffset;
        return;
    }
}

