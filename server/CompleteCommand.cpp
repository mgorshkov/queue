#include "CompleteCommand.h"

CompleteCommand::CompleteCommand(const MessagePtr& aMessage)
{
    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(aMessage);
    if (queueListMessage)
    {
        mCommand = Command::QueueList;
        return;
    }
    auto startQueueSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(aMessage);
    if (startQueueSessionMessage)
    {
        mCommand = Command::StartQueueSession;
        mQueueName = startQueueSessionMessage->mQueueName;
        mOffset = startQueueSessionMessage->mOffset;
        return;
    }
    auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(aMessage);
    if (enqueueMessage)
    {
        mCommand = Command::Enqueue;
        mData = enqueueMessage->mData;
        return;
    }
    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(aMessage);
    if (dequeueMessage)
    {
        mCommand = Command::Dequeue;
        mItem = dequeueMessage->mItem;
        return;
    }
}

