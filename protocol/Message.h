#pragma once

#include <memory>

#include "Defines.h"

enum class MessageType
{
    QueueList,
    StartQueueSession,
    Enqueue,
    Dequeue,
};

struct Message
{
};

using MessagePtr = std::shared_ptr<Message>;

struct QueueListMessage : Message
{
    QueueList mQueueList;

    friend std::istream& operator >> (std::istream& aStream, QueueListMessage& aMessage)
    {
        std::size_t size;
        aStream >> size;
        for (std::size_t i = 0; i < size; ++i)
        {
            DataType data;
            aStream >> data;
            aMessage.mQueueList.emplace_back(data);
        }
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const QueueListMessage& aMessage)
    {
        aStream << aMessage.mQueueList.size();
        for (auto item : aMessage.mQueueList)
            aStream << item;
        return aStream;
    }
};

struct StartQueueSessionMessage : Message
{
    std::string mQueueName;
    std::size_t mOffset;

    friend std::istream& operator >> (std::istream& aStream, StartQueueSessionMessage& aMessage)
    {
        aStream >> aMessage.mQueueName;
        aStream >> aMessage.mOffset;
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const StartQueueSessionMessage& aMessage)
    {
        aStream << aMessage.mQueueName;
        aStream << aMessage.mOffset;
        return aStream;
    }
};

struct DequeueMessage : Message
{
    Item mItem;

    friend std::istream& operator >> (std::istream& aStream, DequeueMessage& aMessage)
    {
        aStream >> aMessage.mItem;
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const DequeueMessage& aMessage)
    {
        aStream << aMessage.mItem;
        return aStream;
    }
};

struct EnqueueMessage : Message
{
    DataType mData;

    friend std::istream& operator >> (std::istream& aStream, EnqueueMessage& aMessage)
    {
        aStream >> aMessage.mData;
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const EnqueueMessage& aMessage)
    {
        aStream << aMessage.mData;
        return aStream;
    }
};
