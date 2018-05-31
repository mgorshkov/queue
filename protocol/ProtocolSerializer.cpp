#include "ProtocolSerializer.h"

#include "assert.h"

void ProtocolSerializer::Serialize(const QueueListMessage& aMessage, std::ostream& aStream)
{
    aStream << static_cast<int>(MessageType::QueueList);
    aStream << aMessage;
}

void ProtocolSerializer::Serialize(const StartQueueSessionMessage& aMessage, std::ostream& aStream)
{
    aStream << static_cast<int>(MessageType::StartQueueSession);
    aStream << aMessage;
}

void ProtocolSerializer::Serialize(const EnqueueMessage& aMessage, std::ostream& aStream)
{
    aStream << static_cast<int>(MessageType::Enqueue);
    aStream << aMessage;
}

void ProtocolSerializer::Serialize(const DequeueMessage& aMessage, std::ostream& aStream)
{
    aStream << static_cast<int>(MessageType::Dequeue);
    aStream << aMessage;
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
