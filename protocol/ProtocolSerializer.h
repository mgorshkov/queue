#pragma once

#include "Defines.h"
#include "Message.h"

class ProtocolSerializer
{
public:
    static void Serialize(MessagePtr aMessage, std::ostream& aStream);

    static MessagePtr Deserialize(std::istream& aStream);
};
