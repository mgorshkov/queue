#pragma once

#include "Defines.h"

#include <boost/iostreams/device/mapped_file.hpp>

class QueueStorage
{
public:
    QueueStorage(const std::string& aStorageFileName);
    ~QueueStorage();

    void AddItem(const Item& aItem);
    DataType GetItem(std::size_t aOffset);

private:
    uintmax_t CalcFileSize() const;

    bool CheckFreeDiskSpace();
    void ShrinkStorage();
    void ShrinkQueue();

    using MappedFile = boost::iostreams::mapped_file;
    struct MappedFileDescriptor
    {
        MappedFile mIndex;
        MappedFile mData;
    };

    std::unique_ptr<MappedFileDescriptor> mMappedFileDescriptor;

    std::string mStorageFileName;
    std::string mIndexFileName;

    static const uintmax_t MaxFileSize = 1024 * 1024 * 1024;

    static const uintmax_t FreeSpaceThreshold = 1024 * 1024 * 1024;

};

