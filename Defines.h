#pragma once

#include <queue>
#include <memory>
#include <string>

using DataType = std::string;

struct Item
{
    DataType mData;
    std::size_t mOffset;
};

using ItemPtr = std::unique_ptr<Item>;
using ItemQueue = std::queue<ItemPtr>;

struct ServerData
{
    std::string mServerIp;
    int mServerPort;
};
