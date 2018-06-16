#pragma once

#include "Defines.h"
#include "QueueStorage.h"
#include <boost/filesystem.hpp>
#include <map>

class Queue
{
public:
    Queue();

    void Load(const boost::filesystem::path& aStorageFolderName);

    void Enqueue(const DataType& aData);
    Item Dequeue(std::size_t aOffset);

private:
    void CreateStorageIfEmpty();
    void CreateStorageByOffset(std::size_t aOffset);

    std::map<uintmax_t, std::unique_ptr<QueueStorage>> mQueueStorage; // minOffset->storage
};

