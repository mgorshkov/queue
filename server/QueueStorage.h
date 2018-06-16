#pragma once

#include "Defines.h"

#include <boost/iostreams/device/mapped_file.hpp>

class QueueStorage
{
public:
    QueueStorage(const std::string& aStorageFileName);
    ~QueueStorage();

    bool AddItem(const DataType& aItem);
    DataType GetItem(std::size_t aOffset);

private:
    bool CheckFreeDiskSpace() const;

    using MappedFile = boost::iostreams::mapped_file;
    struct MappedFileDescriptor
    {
        MappedFile mIndex;
        MappedFile mData;
    };

    std::unique_ptr<MappedFileDescriptor> mMappedFileDescriptor;

    std::string mStorageFileName;
    std::size_t mStorageOffset;

    static const uintmax_t MaxFileSize = 1024 * 1024 * 1024;

    static const uintmax_t FreeSpaceThreshold = 1024 * 1024 * 1024;

};

