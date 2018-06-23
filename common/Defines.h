#pragma once

#include <memory>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include <array>

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
        std::size_t size;
        if (!aStream.read((char*)&size, sizeof(size)))
            return aStream;
        aItem.mData.resize(size);
        if (!aStream.read(&aItem.mData[0], size))
            return aStream;

        if (!aStream.read((char*)&aItem.mOffset, sizeof(aItem.mOffset)))
            return aStream;
        return aStream;
    }

    friend std::ostream& operator << (std::ostream& aStream, const Item& aItem)
    {
        auto size = aItem.mData.size();
        if (!aStream.write((const char*)&size, sizeof(size)))
            return aStream;
        aStream << aItem.mData;
        if (!aStream)
            return aStream;

        aStream.write((char*)&aItem.mOffset, sizeof(aItem.mOffset));
        return aStream;
    }
};

const constexpr std::size_t MaxMessageSize = 256;
using BufferType = std::array<char, MaxMessageSize>;

using QueueList = std::vector<DataType>;

struct ServerData
{
    std::string mHost;
    int mPort;
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
    StartSession,
    Enqueue,
    Dequeue,
};

