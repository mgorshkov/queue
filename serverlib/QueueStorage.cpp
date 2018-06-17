#include <sstream>

#include "QueueStorage.h"

QueueStorage::QueueStorage(const boost::filesystem::path& aStorageFileName)
    : mStorageOffset(0)
    , mDataSizeUsed(nullptr)
    , mIndexSizeUsed(nullptr)
    , mStorageFileName(aStorageFileName)
{
#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::QueueStorage aStorageFileName=" << aStorageFileName << std::endl;
#endif
    std::stringstream offsetStr(aStorageFileName.filename().string());
    offsetStr >> mStorageOffset;

    boost::filesystem::path storageFileNameData(aStorageFileName);
    boost::filesystem::path storageFileNameIndex(aStorageFileName);

    storageFileNameData.replace_extension("data");
    storageFileNameIndex.replace_extension("index");

    boost::iostreams::mapped_file_params mappedFileParamsData;
    mappedFileParamsData.path          = storageFileNameData.string();
    if (!boost::filesystem::exists(storageFileNameData))
        mappedFileParamsData.new_file_size = MaxFileSize;
    mappedFileParamsData.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    boost::iostreams::mapped_file_params mappedFileParamsIndex;
    mappedFileParamsIndex.path          = storageFileNameIndex.string();
    if (!boost::filesystem::exists(storageFileNameIndex))
        mappedFileParamsIndex.new_file_size = MaxFileSize;
    mappedFileParamsIndex.flags         = boost::iostreams::mapped_file::mapmode::readwrite;

    mMappedFileDescriptor =
        std::make_unique<MappedFileDescriptor>(
            MappedFileDescriptor{MappedFile(mappedFileParamsIndex), MappedFile(mappedFileParamsData)});

    mDataSizeUsed = reinterpret_cast<std::size_t*>(mMappedFileDescriptor->mData.data());
    mIndexSizeUsed = reinterpret_cast<std::size_t*>(mMappedFileDescriptor->mIndex.data());
}

QueueStorage::~QueueStorage()
{
}

bool QueueStorage::AddItem(const DataType& aData)
{
    std::size_t newDataSize = *mDataSizeUsed + aData.size();
//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, newDataSize=" << newDataSize << std::endl;
//#endif
    if (newDataSize > MaxFileSize || !CheckFreeDiskSpace())
        return false;

//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mDataSizeUsed before=" << *mDataSizeUsed << std::endl;
//#endif

    std::size_t newIndexSize = *mIndexSizeUsed + sizeof(std::size_t);
//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, newIndexSize=" << newIndexSize << std::endl;
//#endif
    if (newIndexSize > MaxFileSize || !CheckFreeDiskSpace())
        return false;

//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mIndexSizeUsed before=" << *mIndexSizeUsed << std::endl;
//#endif
    memcpy(mMappedFileDescriptor->mIndex.data() + *mIndexSizeUsed + sizeof(*mIndexSizeUsed), mDataSizeUsed, sizeof(*mDataSizeUsed));
    strcpy(mMappedFileDescriptor->mData.data() + *mDataSizeUsed + sizeof(*mDataSizeUsed), aData.c_str());
    *mDataSizeUsed += (aData.size() + 1);
    *mIndexSizeUsed += sizeof(mDataSizeUsed);

//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mDataSizeUsed after=" << *mDataSizeUsed << std::endl;
//#endif
//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mIndexSizeUsed after=" << *mIndexSizeUsed << std::endl;
//#endif

    return true;
}

DataType QueueStorage::GetItem(std::size_t aOffset)
{
    assert (aOffset >= mStorageOffset);
    std::size_t* baseIndex = reinterpret_cast<std::size_t*>(mMappedFileDescriptor->mIndex.data());
    std::size_t dataOffset = baseIndex[aOffset - mStorageOffset];
//#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::GetItem, dataOffset=" << dataOffset << std::endl;
//#endif
    char* baseData = mMappedFileDescriptor->mData.data();
    return DataType(baseData + dataOffset);
}

bool QueueStorage::CheckFreeDiskSpace() const
{
    try
    {
        boost::filesystem::path storageFileNameData(mStorageFileName);
        storageFileNameData.replace_extension("data");
        boost::filesystem::space_info info = boost::filesystem::space(storageFileNameData);
        return info.available > FreeSpaceThreshold;
    }
    catch (boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return true;
}

