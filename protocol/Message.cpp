#include "Message.h"

void Dump(MessagePtr message)
{
    if (!message)
    {
        std::cout << "Null message" << std::endl;
        return;
    }
    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(message);
    if (queueListMessage)
    {
        std::cout << "QueueListMessage:" << std::endl;
        for (auto str : queueListMessage->mQueueList)
            std::cout << str << std::endl;
        return;
    }
    auto startQueueSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(message);
    if (startQueueSessionMessage)
    {
        std::cout << "StartQueueSessionMessage:" << std::endl;
        std::cout << "QueueName:" << startQueueSessionMessage->mQueueName << std::endl;
        std::cout << "Offset:" << startQueueSessionMessage->mOffset << std::endl;
        return;
    }
    auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(message);
    if (enqueueMessage)
    {
        std::cout << "Enqueue:" << std::endl;
        std::cout << "Data:" << enqueueMessage->mData << std::endl;
        return;
    }
    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(message);
    if (dequeueMessage)
    {
        std::cout << "Dequeue:" << std::endl;
        std::cout << "Item:" << dequeueMessage->mItem << std::endl;
        return;
    }
}

