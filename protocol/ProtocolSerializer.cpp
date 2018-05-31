#include "ProtocolSerializer.h"

#include "assert.h"

ProtocolSerializer::ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc)
    : mReadFunc(aReadFunc)
    , mWriteFunc(aWriteFunc)
{
}

void ProtocolSerializer::Serialize(const QueueListMessage& aMessage)
{
    ba::streambuf stream;
    std::ostream line(&stream);
    line << static_cast<int>(MessageType::QueueList);
    line << aMessage;
    mWriteFunc(stream);
}

void ProtocolSerializer::Serialize(const StartQueueSessionMessage& aMessage)
{
    ba::streambuf stream;
    std::ostream line(&stream);
    line << static_cast<int>(MessageType::StartQueueSession);
    line << aMessage;
    mWriteFunc(stream);
}

void ProtocolSerializer::Serialize(const EnqueueMessage& aMessage)
{
    ba::streambuf stream;
    std::ostream line(&stream);
    line << static_cast<int>(MessageType::Enqueue);
    line << aMessage;
    mWriteFunc(stream);
}

void ProtocolSerializer::Serialize(const DequeueMessage& aMessage)
{
    ba::streambuf stream;
    std::ostream line(&stream);
    line << static_cast<int>(MessageType::Dequeue);
    line << aMessage;
    mWriteFunc(stream);
}

MessagePtr ProtocolSerializer::Deserialize()
{
    ba::streambuf stream;
    mReadFunc(stream);
    std::istream line(&stream);
    int messageType;
    line >> messageType;
    switch (static_cast<MessageType>(messageType))
    {
    case MessageType::QueueList:
    {
        QueueListMessage message;
        line >> message;
        return std::make_shared<QueueListMessage>(message);
    }
    case MessageType::StartQueueSession:
    {
        StartQueueSessionMessage message;
        line >> message;
        return std::make_shared<StartQueueSessionMessage>(message);
    }
    case MessageType::Enqueue:
    {
        EnqueueMessage message;
        line >> message;
        return std::make_shared<EnqueueMessage>(message);
    }
    case MessageType::Dequeue:
    {
        DequeueMessage message;
        line >> message;
        return std::make_shared<DequeueMessage>(message);
    }
    default:
    {
        assert(false && "unknown message");
        return nullptr;
    }
    }
}
