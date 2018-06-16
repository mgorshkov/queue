#include <sstream>
#include <boost/filesystem.hpp>

#include "QueueStorage.h"

QueueStorage::QueueStorage(const std::string& aStorageFileName)
    : mStorageOffset(0)
    , mStorageFileName(aStorageFileName)
{
    std::stringstream offsetStr(aStorageFileName);
    offsetStr >> mStorageOffset;

    boost::iostreams::mapped_file_params mappedFileParamsData;
    mappedFileParamsData.path          = aStorageFileName + ".data";
    mappedFileParamsData.new_file_size = 0;
    mappedFileParamsData.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    boost::iostreams::mapped_file_params mappedFileParamsIndex;
    mappedFileParamsIndex.path          = aStorageFileName + ".index";
    mappedFileParamsIndex.new_file_size = 0;
    mappedFileParamsIndex.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    mMappedFileDescriptor =
        std::make_unique<MappedFileDescriptor>(MappedFileDescriptor{MappedFile(mappedFileParamsIndex),
                                               MappedFile(mappedFileParamsData)});
}

QueueStorage::~QueueStorage()
{
}

bool QueueStorage::AddItem(const DataType& aItem)
{
    if (mMappedFileDescriptor->mData.size() + aItem.size() > MaxFileSize || !CheckFreeDiskSpace())
        return false;

    auto newDataOffset = mMappedFileDescriptor->mData.size();
    auto newIndexOffset = mMappedFileDescriptor->mIndex.size();
    memcpy(mMappedFileDescriptor->mIndex.data() + newIndexOffset, &newDataOffset, sizeof(newDataOffset));
    strcpy(mMappedFileDescriptor->mData.data() + newDataOffset, aItem.c_str());

    return true;
}

DataType QueueStorage::GetItem(std::size_t aOffset)
{
    assert (aOffset >= mStorageOffset);
    std::size_t* baseIndex = reinterpret_cast<std::size_t*>(mMappedFileDescriptor->mIndex.data());
    std::size_t dataOffset = baseIndex[aOffset - mStorageOffset];
    char* baseData = mMappedFileDescriptor->mData.data();
    return DataType(baseData + dataOffset);
}

bool QueueStorage::CheckFreeDiskSpace() const
{
    try
    {
        boost::filesystem::space_info info = boost::filesystem::space(mStorageFileName);
        return info.available > FreeSpaceThreshold;
    }
    catch (boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return true;
}

