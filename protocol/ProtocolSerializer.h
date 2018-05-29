#pragma once

#include "Message.h"

class ProtocolSerializer
{
public:
    using ReadFunc = std::function<std::size_t(ba::buffer& aBuffer)>;
    using WriteFunc = std::function<void(const ba::buffer& aBuffer)>;

    ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc);

    void Serialize(MessagePtr aMessage);

    MessagePtr Deserialize();

private:
    ReadFunc mReadFunc;
    WriteFunc mWriteFunc;
};
