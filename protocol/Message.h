#pragma once

#include <memory>

#include "Defines.h"

enum class MessageType
{
    QueueList,
    StartQueueSession,
    Dequeue,
    Enqueue,
};

struct QueueListMessage
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
    }

    friend std::ostream& operator << (std::ostream& aStream, const QueueListMessage& aMessage)
    {
        aStream << aMessage.mQueueList.size();
        for (auto item : aMessage.mQueueList)
            aStream << item;
    }
};

struct DequeueMessage : Message
{
    Item mItem;

    friend std::istream& operator >> (std::istream& aStream, DequeueMessage& aMessage)
    {
        aStream >> aMessage.mItem;
    }

    friend std::ostream& operator << (std::ostream& aStream, const DequeueMessage& aMessage)
    {
        aStream << aMessage.mItem;
    }
};

struct StartQueueSessionMessage : Message
{
    std::string mQueueName;
    std::size_t mOffset;

    std::istream& operator >> (std::istream& aStream)
    {
        Message::operator >> (aStream);

        aStream >> mQueueName;
        aStream >> mOffset;
    }

    std::ostream& operator << (std::ostream& aStream)
    {
        Message::operator << (aStream);

        aStream << mQueueName;
        aStream << mOffset;
    }
};

