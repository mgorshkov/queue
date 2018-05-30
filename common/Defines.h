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
    }

    friend std::ostream& operator << (std::ostream& aStream, const Item& aItem)
    {
        aStream << aItem.mData;
        aStream << aItem.mOffset;
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
    modeSync,
    modeAsync
};

enum class ConsumerProducerMode
{
    modeConsumer,
    modeProducer
};
