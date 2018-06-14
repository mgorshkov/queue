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

    , mMappedFileDescriptor(boost::iostreams::mapped_file(, ),
        boost::iostreams::mapped_file(mStorageFileName + ".data", MaxFileSize))
{
}

QueueStorage::~QueueStorage()
{
}

void QueueStorage::AddItem(const Item& aItem)
{

    if (boost::filesystem::file_size(mCurrentFileName) > MaxFileSize)
        FoldStorage(offset + 1);

    if (!CheckFreeDiskSpace())
    {
        ShrinkStorage();
        ShrinkQueue();
    }
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
