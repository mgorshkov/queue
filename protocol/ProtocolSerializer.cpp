#include "ProtocolSerializer.h"

ProtocolSerializer::ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc)
    : mReadFunc(aReadFunc)
    , mWriteFunc(aWriteFunc)
{
}

void ProtocolSerializer::Serialize(MessagePtr aMessage)
{
    ba::streambuf stream;
    stream << aMessage;
    mWriteFunc(stream);
}

MessagePtr ProtocolSerializer::Deserialize()
{
    ba::streambuf stream;
    mReadFunc(stream);
    make_shared<
}
