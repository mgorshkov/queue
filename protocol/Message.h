#pgarma once

#include <memory>

enum class MessageType
{
    QueueList,
    StartQueueSession,
    Dequeue,
    Enqueue,
};

struct Message
{
    MessageType mMessageType;
};

using MessagePtr = std::shared_ptr<Message>;

struct QueueListMessage : Message
{
    QueueList mQueueList;
};

std::istream& operator << (std::istream& QueueListMessage)
{
}

std::ostream& operator >> (std::ostream& QueueListMessage)
{
}

struct DequeueMessage : Message
{
    Item mItem;
};

struct StartQueueSessionMessage : Message
{
    std::string mQueueName;
    std::size_t mOffset;
};
    
