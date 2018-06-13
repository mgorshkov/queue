#include <iostream>

#include "QueueStorage.h"

QueueStorage::QueueStorage(const boost::filesystem::path& aStorageFolderName)
    : mStorageFolderName(aStorageFolderName)
{
    Init();
}

QueueStorage::~QueueStorage()
{
}

void QueueStorage::Init() const
{
    for (boost::filesystem::directory_iterator it(mStorageFilePath); it != boost::filesystem::directory_iterator(); ++it)
    {
        if (!boost::filesystem::is_regular_file(it->status()))
            continue;

        if (it->path().extension() != "index")
            continue;

        auto fileName = it->path().filename().string();

        std::stringstream offsetStr(fileName);
        uintmax_t offset;
        offsetStr >> offset;

        MappedFileDescriptor descriptor{boost::iostreams::mapped_file(fileName + ".index"),
            boost::iostreams::mapped_file(fileName + ".data"), MaxFileSize};
        mStreams.insert(std::make_pair(offset, descriptor));
    }
}

void QueueStorage::FoldStorage(std::size_t aOffset)
{
    mStream.close();

    std::stringstream offsetStr;
    offsetStr << aOffset;

    mStorageFileName = offsetStr.str();

//    mStream.open(mStorageFileName.string(), std::fstream::out | std::fstream::app);
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
