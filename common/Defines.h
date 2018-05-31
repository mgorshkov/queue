#pragma once

#include <queue>
#include <memory>
#include <string>
#include <cstddef>

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

using ItemPtr = std::unique_ptr<Item>;
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

enum class ConsumerProducerMode
{
    Consumer,
    Producer,
};

enum class Command
{
    QueueList,
    StartQueueSession,
    Enqueue,
    Dequeue,
};

struct CompleteCommand
{
    Command mCommand;
    std::string mQueueName;
    std::size_t mOffset;
    DataType mData;
};

struct CompleteOperationStatus
{
    QueueList mQueueList;
    Item mItem;
};

using CompleteOperationStatuses = std::list<CompleteOperationStatus>;
