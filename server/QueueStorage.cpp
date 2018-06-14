#include <iostream>

#include "QueueStorage.h"

QueueStorage::QueueStorage(const std::string& aStorageFileName)
    : mStorageFileName(aStorageFileName)
{
    boost::iostreams::mapped_file_params mappedFileParamsData;
    mappedFileParamsData.path          = mStorageFileName + ".data";
    mappedFileParamsData.new_file_size = MaxFileSize;
    mappedFileParamsData.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    boost::iostreams::mapped_file_params mappedFileParamsIndex;
    mappedFileParamsIndex.path          = mStorageFileName + ".index";
    mappedFileParamsIndex.new_file_size = MaxFileSize;
    mappedFileParamsIndex.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    mMappedFileDescriptor =
        std::make_unique<MappedFileDescriptor>(MappedFile(mappedFileParamsIndex),
                                               MappedFile(mappedFileParamsData));
}

QueueStorage::~QueueStorage()
{
}

void QueueStorage::AddItem(const Item& aItem)
{
    memcpy(mMappedFileDescriptor->mIndex.data() + aItem.mOffset)
    strcpy(mMappedFileDescriptor->mData.data() + aItem.mOffset, aItem.mData.c_str());
    if (boost::filesystem::file_size(mCurrentFileName) > MaxFileSize)
        FoldStorage(offset + 1);

    if (!CheckFreeDiskSpace())
    {
        ShrinkStorage();
        ShrinkQueue();
    }
}

DataType QueueStorage::GetItem(std::size_t aOffset)
{
    std::size_t dataOffset = mMappedFileDescriptor.mIndex[aOffset];
    std::size_t length = strlen(mMappedFileDescriptor.mData.ptr() + dataOffset);
    return DataType(mMappedFileDescriptor.mData.ptr() + dataOffset, length);
}

uintmax_t QueueStorage::CalcFileSize() const
{
    try
    {
        boost::filesystem::space_info info = boost::filesystem::space(mStorageFolderName);
        return info.available / 10 > MaxFileSize ? MaxFileSize : info.available / 10;
    }
    catch (boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return MaxFileSize;
}

bool QueueStorage::CheckFreeDiskSpace() const
{
    try
    {
        boost::filesystem::space_info info = boost::filesystem::space(mStorageFolderName);
        return info.available > FreeSpaceThreshold;
    }
    catch (boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return true;
}

void QueueStorage::ShrinkStorage()
{
}

void QueueStorage::ShrinkQueue()
{
    mShrink->Shrink();
}
