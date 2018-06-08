#pragma once

#include "Defines.h"
#include "IShrink.h"
#include "QueueStorage.h"
#include <boost/filesystem.hpp>

class Queue : public IShrink
{
public:
    Queue();

    void New(const boost::filesystem::path& aStorageFileName);
    void Load(const boost::filesystem::path& aStorageFileName);

    void Start();

    void Enqueue(const DataType &aData);
    Item Dequeue(std::size_t aOffset);

    void Shrink();

private:
    ItemQueue mQueue;
    std::size_t mOffset;
    QueueStorage mQueueStorage;
};

