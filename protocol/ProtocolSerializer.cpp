#pragma once

#include "ProtocolSerializer.h"

ProtocolSerializer::ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc)
    : mReadFunc(aReadFunc)
    , mWriteFunc(aWriteFunc)
{
}

void ProtocolSerializer::Serialize(MessagePtr aMessage)
{
    ba::buffer buffer(&aMessage, sizeof(aMessage));
    mWriteFunc(buffer);
}

MessagePtr ProtocolSerializer::Deserialize()
{
    ba::buffer buffer(&aMessage, sizeof(aMessage));
    mWriteFunc(buffer);
}
