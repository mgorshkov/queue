#pragma once

#include <functional>

#include "Defines.h"
#include "Message.h"

class ProtocolSerializer
{
public:
    using ReadFunc = std::function<std::size_t(ba::streambuf&)>;
    using WriteFunc = std::function<void(const ba::streambuf&)>;

    ProtocolSerializer(ReadFunc aReadFunc, WriteFunc aWriteFunc);

    void Serialize(const QueueListMessage& aMessage);
    void Serialize(const StartQueueSessionMessage& aMessage);
    void Serialize(const EnqueueMessage& aMessage);
    void Serialize(const DequeueMessage& aMessage);

    MessagePtr Deserialize();

private:
    ReadFunc mReadFunc;
    WriteFunc mWriteFunc;
};
