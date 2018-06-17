#include "CommandContext.h"

CommandContext::CommandContext(StartQueueSessionMessagePtr aMessage)
{
    mQueueName = aMessage->mQueueName;
    mOffset = aMessage->mOffset;
}
