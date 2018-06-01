#include "ProtocolSerializer.h"

#include "assert.h"

void ProtocolSerializer::Serialize(MessagePtr aMessage, std::ostream& aStream)
{
    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(aMessage);
    if (queueListMessage)
    {
        aStream << static_cast<int>(MessageType::QueueList);
        aStream << *queueListMessage;
        return;
    }
    auto startQueueSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(aMessage);
    if (startQueueSessionMessage)
    {
        aStream << static_cast<int>(MessageType::StartQueueSession);
        aStream << *startQueueSessionMessage;
        return;
    }
    auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(aMessage);
    if (enqueueMessage)
    {
        aStream << static_cast<int>(MessageType::Enqueue);
        aStream << *enqueueMessage;
        return;
    }
    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(aMessage);
    if (dequeueMessage)
    {
        aStream << static_cast<int>(MessageType::Dequeue);
        aStream << *dequeueMessage;
        return;
    }
}

void ProtocolSerializer::Serialize(CompleteOperationStatusPtr aMessage, std::ostream& aStream)
{
    auto queueListOperationStatus = std::dynamic_pointer_cast<QueueListOperationStatus>(aMessage);
    if (queueListOperationStatus)
    {
        aStream << static_cast<int>(OperationStatus::QueueList);
        aStream << *queueListOperationStatus;
        return;
    }
    auto itemOperationStatus = std::dynamic_pointer_cast<ItemOperationStatus>(aMessage);
    if (itemOperationStatus)
    {
        aStream << static_cast<int>(OperationStatus::Item);
        aStream << *itemOperationStatus;
        return;
    }
}

MessagePtr ProtocolSerializer::Deserialize(std::istream& aStream)
{
    int messageType;
    aStream >> messageType;
    switch (static_cast<MessageType>(messageType))
    {
    case MessageType::QueueList:
    {
        QueueListMessage message;
        aStream >> message;
        return std::make_shared<QueueListMessage>(message);
    }
    case MessageType::StartQueueSession:
    {
        StartQueueSessionMessage message;
        aStream >> message;
        return std::make_shared<StartQueueSessionMessage>(message);
    }
    case MessageType::Enqueue:
    {
        EnqueueMessage message;
        aStream >> message;
        return std::make_shared<EnqueueMessage>(message);
    }
    case MessageType::Dequeue:
    {
        DequeueMessage message;
        aStream >> message;
        return std::make_shared<DequeueMessage>(message);
    }
    default:
    {
        assert(false && "unknown message");
        return nullptr;
    }
    }
}
