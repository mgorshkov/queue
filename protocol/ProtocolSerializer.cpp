#include "ProtocolSerializer.h"

ProtocolSerializer::ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc)
    : mReadFunc(aReadFunc)
    , mWriteFunc(aWriteFunc)
{
}

void ProtocolSerializer::Serialize(const QueueListMessage& aMessage)
{
    ba::streambuf stream;
    std::ostream line(&stream);
    line << MessageType::QueueList;
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
    switch (message.mMessageType)
    {
    case MessageType::QueueList:
    {
        QueueListMessage message;
        line >> message;
        return std::make_shared<QueueListMessage>(message);
    }
    }
}
