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
    InitStorageOffset();

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

    auto data = mMappedFileDescriptor->mData.data();
    auto index = mMappedFileDescriptor->mIndex.data();

    mDataSizeUsed = reinterpret_cast<uint32_t*>(data);
    mIndexSizeUsed = reinterpret_cast<uint32_t*>(index);

    mBaseData = data + sizeof(uint32_t);
    mBaseIndex = reinterpret_cast<uint32_t*>(index) + 1;
}

QueueStorage::~QueueStorage()
{
}

QueueStorage::AddStatus QueueStorage::AddData(const DataType& aData)
{
    uint32_t newDataSize = *mDataSizeUsed + aData.size() + 1;
#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, newDataSize=" << newDataSize << std::endl;
#endif
    if (newDataSize > MaxFileSize)
        return AddStatus::FileFull;

    if ((*mIndexSizeUsed + 1) * sizeof(uint32_t) > MaxFileSize)
        return AddStatus::FileFull;

    if (!CheckFreeDiskSpace())
        return AddStatus::DiskFull;

#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mDataSizeUsed before=" << *mDataSizeUsed << std::endl;
#endif

    mBaseIndex[(*mIndexSizeUsed)++] = *mDataSizeUsed;

    strcpy(mBaseData + *mDataSizeUsed, aData.c_str());
    *mDataSizeUsed = newDataSize;

#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mDataSizeUsed after=" << *mDataSizeUsed << std::endl;
#endif
#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::AddItem, mIndexSizeUsed after=" << *mIndexSizeUsed << std::endl;
#endif

    return AddStatus::Ok;
}

bool QueueStorage::GetData(std::size_t aOffset, DataType& aData)
{
    assert (aOffset >= mStorageOffset);
    std::size_t indexOffset = aOffset - mStorageOffset;
    if (indexOffset >= *mIndexSizeUsed)
        return false;
    uint32_t dataOffset = mBaseIndex[indexOffset];
#ifdef DEBUG_PRINT
    std::cout << "QueueStorage::GetItem, dataOffset=" << dataOffset << std::endl;
#endif
    aData = DataType(mBaseData + dataOffset);
    return true;
}

std::size_t QueueStorage::GetNextOffset() const
{
    return *mIndexSizeUsed + mStorageOffset;
}

void QueueStorage::InitStorageOffset()
{
    std::stringstream offsetStr(mStorageFileName.filename().string());

    offsetStr >> std::hex >> mStorageOffset;
}

bool QueueStorage::CheckFreeDiskSpace() const
{
    try
    {
        boost::filesystem::path dir = mStorageFileName.parent_path();
        boost::filesystem::space_info info = boost::filesystem::space(dir);
        return info.available > FreeSpaceThreshold;
    }
    catch (boost::filesystem::filesystem_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return true;
}

