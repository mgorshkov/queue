#pragma once

#include "Defines.h"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

class QueueStorage
{
public:
    QueueStorage(const boost::filesystem::path& aStorageFilePath);
    ~QueueStorage();

    enum class AddStatus
    {
        Ok,
        FileFull,
        DiskFull,
    };
    AddStatus AddData(const DataType& aData);
    bool GetData(std::size_t aOffset, DataType& aData);

    std::size_t GetNextOffset() const;

private:
    void InitStorageOffset();
    bool CheckFreeDiskSpace() const;

    using MappedFile = boost::iostreams::mapped_file;
    struct MappedFileDescriptor
    {
        MappedFile mIndex;
        MappedFile mData;
    };

    std::unique_ptr<MappedFileDescriptor> mMappedFileDescriptor;

    boost::filesystem::path mStorageFilePath;
    std::size_t mStorageOffset;

    uint32_t* mDataSizeUsed;
    uint32_t* mIndexSizeUsed;

    char* mBaseData;
    uint32_t* mBaseIndex;

    static const uintmax_t MaxFileSize = 1024 * 1024 * 1024;

    // multiple by 2 because of 2 files: data and index
    static const uintmax_t FreeSpaceThreshold = 2 * MaxFileSize;
};

