#pragma once

#include "Defines.h"
#include "Message.h"

class ProtocolSerializer
{
public:
    static void Serialize(const QueueListMessage& aMessage, std::ostream &aStream);
    static void Serialize(const StartQueueSessionMessage& aMessage, std::ostream &aStream);
    static void Serialize(const EnqueueMessage& aMessage, std::ostream &aStream);
    static void Serialize(const DequeueMessage& aMessage, std::ostream &aStream);

    static MessagePtr Deserialize(std::istream &aStream);
};
