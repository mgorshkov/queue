#include "ProtocolSerializer.h"

#include "assert.h"

void ProtocolSerializer::Serialize(MessagePtr aMessage, std::ostream& aStream)
{
    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(aMessage);
    if (queueListMessage)
    {
        auto type = static_cast<int>(MessageType::QueueList);
        if (!aStream.write((const char*)&type, sizeof(type)))
            return;
        aStream << *queueListMessage;
        return;
    }
    auto startQueueSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(aMessage);
    if (startQueueSessionMessage)
    {
        auto type = static_cast<int>(MessageType::StartQueueSession);
        if (!aStream.write((const char*)&type, sizeof(type)))
            return;
        aStream << *startQueueSessionMessage;
        return;
    }
    auto enqueueMessage = std::dynamic_pointer_cast<EnqueueMessage>(aMessage);
    if (enqueueMessage)
    {
        auto type = static_cast<int>(MessageType::Enqueue);
        if (!aStream.write((const char*)&type, sizeof(type)))
            return;
        aStream << *enqueueMessage;
        return;
    }
    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(aMessage);
    if (dequeueMessage)
    {
        auto type = static_cast<int>(MessageType::Dequeue);
        if (!aStream.write((const char*)&type, sizeof(type)))
            return;
        aStream << *dequeueMessage;
        return;
    }
}

MessagePtr ProtocolSerializer::Deserialize(std::istream& aStream)
{
    int messageType;
    aStream.read((char*)&messageType, sizeof(messageType));

    switch (static_cast<MessageType>(messageType))
    {
    case MessageType::QueueList:
    {
        QueueListMessage message;
        aStream >> message;
        if (!aStream)
            return nullptr;
        return std::make_shared<QueueListMessage>(message);
    }
    case MessageType::StartQueueSession:
    {
        StartQueueSessionMessage message;
        aStream >> message;
        if (!aStream)
            return nullptr;
        return std::make_shared<StartQueueSessionMessage>(message);
    }
    case MessageType::Enqueue:
    {
        EnqueueMessage message;
        aStream >> message;
        if (!aStream)
            return nullptr;
        return std::make_shared<EnqueueMessage>(message);
    }
    case MessageType::Dequeue:
    {
        DequeueMessage message;
        aStream >> message;
        if (!aStream)
            return nullptr;
        return std::make_shared<DequeueMessage>(message);
    }
    default:
    {
        return nullptr;
    }
    }
}
