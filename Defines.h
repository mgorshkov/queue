#pragma once

#include <queue>
#include <memory>
#include <string>

#include <boost/asio.h>

namespace ba = boost::asio;
using DataType = std::string;

struct Item
{
    DataType mData;
    std::size_t mOffset;
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
