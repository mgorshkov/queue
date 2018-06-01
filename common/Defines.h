#pragma once

#include <queue>
#include <memory>
#include <string>
#include <cstddef>
#include <list>

#include <boost/asio.hpp>

namespace ba = boost::asio;
using DataType = std::string;

struct Item
{
    DataType mData;
    std::size_t mOffset;

    friend std::istream& operator >> (std::istream& aStream, Item& aItem)
    {
        aStream >> aItem.mData;
        aStream >> aItem.mOffset;
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const Item& aItem)
    {
        aStream << aItem.mData;
        aStream << aItem.mOffset;
        return aStream;
    }
};

using ItemPtr = std::shared_ptr<Item>;
using ItemQueue = std::queue<ItemPtr>;

using QueueList = std::vector<DataType>;

struct ServerData
{
    std::string mServerIp;
    int mServerPort;
};

enum class SyncAsyncMode
{
    Sync,
    Async,
};

std::istream& operator >> (std::istream& aStream, SyncAsyncMode& aMode)
{
    std::string name;
    aStream >> name;
    aMode = (name == "sync" ?
        SyncAsyncMode::Sync : SyncAsyncMode::Async);
    return aStream;
}

std::ostream& operator << (std::ostream& aStream, const SyncAsyncMode& aMode)
{
    std::string name = aMode == SyncAsyncMode::Sync ? "sync" : "async";
    aStream << name;
    return aStream;
}

enum class ConsumerProducerMode
{
    Consumer,
    Producer,
};

std::istream& operator >> (std::istream& aStream, ConsumerProducerMode& aMode)
{
    std::string name;
    aStream >> name;
    aMode = (name == "consumer" ?
        ConsumerProducerMode::Consumer : ConsumerProducerMode::Producer);
    return aStream;
}

std::ostream& operator << (std::ostream& aStream, const ConsumerProducerMode& aMode)
{
    std::string name = aMode == ConsumerProducerMode::Consumer ? "consumer" : "producer";
    aStream << name;
    return aStream;
}

enum class Command
{
    QueueListRequest,
    StartQueueSessionRequest,
    EnqueueRequest,
    DequeueRequest,
    QueueListResponse,
    DequeueResponse,
};

struct QueueListOperationStatus : Message
{
    QueueListOperationStatus(const QueueList& aQueueList)
        : mQueueList(aQueueList)
    {
    }
    QueueList mQueueList;
};

using QueueListOperationStatusPtr = std::shared_ptr<QueueListOperationStatus>;

struct ItemOperationStatus : CompleteOperationStatus
{
    ItemOperationStatus(const Item& aItem)
        : mItem(aItem)
    {
    }
    Item mItem;
};

using ItemOperationStatusPtr = std::shared_ptr<ItemOperationStatus>;

using CompleteOperationStatuses = std::list<CompleteOperationStatusPtr>;
