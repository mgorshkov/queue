#pragma once

#include "Defines.h"
#include "QueueStorage.h"
#include <boost/filesystem.hpp>
#include <map>

class Queue
{
public:
    Queue(const boost::filesystem::path& aStorageFolderName);

    void Load();

    void Enqueue(const DataType& aData);
    bool Dequeue(std::size_t aOffset, DataType& aData);

private:
    void CreateStorageIfEmpty();
    void DeleteFirstStorage();

    boost::filesystem::path GetStorageFileNameByOffset(std::size_t aOffset);
    void CreateStorageByOffset(std::size_t aOffset);

    boost::filesystem::path mStorageFolderName;
    std::map<uintmax_t, std::unique_ptr<QueueStorage>> mQueueStorage; // minOffset->storage
};

