#pragma once

#include "Defines.h"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

class QueueStorage
{
public:
    QueueStorage(const boost::filesystem::path& aStorageFolderName);
    ~QueueStorage();

private:
    uintmax_t CalcFileSize() const;

    void FoldStorage(std::size_t aOffset);
    bool CheckFreeDiskSpace();
    void ShrinkStorage();
    void ShrinkQueue();

    using MappedFile = boost::iostreams::mapped_file;
    struct MappedFileDescriptor
    {
        MappedFile mIndex;
        MappedFile mData;
    };
    std::map<uintmax_t, MappedFileDescriptor>> mFiles;

    boost::filesystem::path mStorageFolderName;

    boost::filesystem::path mStorageFileName;
    boost::filesystem::path mIndexFileName;

    static const uintmax_t MaxFileSize = 1024 * 1024 * 1024;

    static const uintmax_t FreeSpaceThreshold = 1024 * 1024 * 1024;

};

