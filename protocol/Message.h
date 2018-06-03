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
    virtual ~Message() = default;
};

using MessagePtr = std::shared_ptr<Message>;
using MessagePtrs = std::list<MessagePtr>;

struct QueueListMessage : Message
{
    QueueListMessage(const QueueList& aQueueList = QueueList{})
        : mQueueList(aQueueList)
    {
    }

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

using QueueListMessagePtr = std::shared_ptr<QueueListMessage>;

struct StartQueueSessionMessage : Message
{
    StartQueueSessionMessage(
        const std::string& aQueueName = std::string{},
        std::size_t aOffset = 0)
        : mQueueName(aQueueName)
        , mOffset(aOffset)
    {
    }

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

using StartQueueSessionMessagePtr = std::shared_ptr<StartQueueSessionMessage>;

struct DequeueMessage : Message
{
    DequeueMessage(const Item& aItem = Item{})
        : mItem(aItem)
    {
    }

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

using DequeueMessagePtr = std::shared_ptr<DequeueMessage>;

struct EnqueueMessage : Message
{
    EnqueueMessage(const DataType& aData = DataType{})
        : mData(aData)
    {
    }

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

using EnqueueMessagePtr = std::shared_ptr<EnqueueMessage>;

