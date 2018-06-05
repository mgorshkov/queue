#include "QueueStorge.h"

QueueStorage::QueueStorage()
    : mThread(&QueueStorage::ThreadProc)
{
}

void QueueStorage::Load(const std::string& aStorageFileName)
{
    mStream.open(aStorageFileName);
}

void QueueStorage::AddItem(const Item& aItem)
{
    mQueue.push_back(aItem);
}

void QueueStorage::ThreadProc()
{
    mStream << aItem;
}


