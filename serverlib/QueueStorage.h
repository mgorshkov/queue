#pragma once

#include "Defines.h"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

class QueueStorage
{
public:
    QueueStorage(const boost::filesystem::path& aStorageFileName);
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

    boost::filesystem::path mStorageFileName;
    std::size_t mStorageOffset;
    std::size_t* mDataSizeUsed;
    std::size_t* mIndexSizeUsed;

    static const uintmax_t MaxFileSize = 1024 * 1024 * 1024;

    static const uintmax_t FreeSpaceThreshold = 1024 * 1024 * 1024;
};

