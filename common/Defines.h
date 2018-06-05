#pragma once

#include <deque>
#include <memory>
#include <string>
#include <cstddef>
#include <list>

#include <boost/asio.hpp>

namespace ba = boost::asio;
using DataType = std::string;

struct Item
{
    Item(const DataType& aData = DataType(), std::size_t aOffset = 0)
        : mData(aData)
        , mOffset(aOffset)
    {
    }

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

using ItemQueue = std::deque<Item>;

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

inline std::istream& operator >> (std::istream& aStream, SyncAsyncMode& aMode)
{
    std::string name;
    aStream >> name;
    aMode = (name == "sync" ?
        SyncAsyncMode::Sync : SyncAsyncMode::Async);
    return aStream;
}

inline std::ostream& operator << (std::ostream& aStream, const SyncAsyncMode& aMode)
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

inline std::istream& operator >> (std::istream& aStream, ConsumerProducerMode& aMode)
{
    std::string name;
    aStream >> name;
    aMode = (name == "consumer" ?
        ConsumerProducerMode::Consumer : ConsumerProducerMode::Producer);
    return aStream;
}

inline std::ostream& operator << (std::ostream& aStream, const ConsumerProducerMode& aMode)
{
    std::string name = aMode == ConsumerProducerMode::Consumer ? "consumer" : "producer";
    aStream << name;
    return aStream;
}

enum class Command
{
    QueueList,
    Enqueue,
    Dequeue,
};

