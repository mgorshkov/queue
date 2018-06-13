#pragma once

#include "Defines.h"
#include "QueueStorage.h"
#include <boost/filesystem.hpp>

class Queue
{
public:
    Queue();

    void CreateStorage(const boost::filesystem::path& aStorageFolderName);
    void Load(const boost::filesystem::path& aStorageFolderName);

    void Enqueue(const DataType& aData);
    Item Dequeue(std::size_t aOffset);

private:
    std::size_t mOffset;
    std::unique_ptr<QueueStorage> mQueueStorage;
};

