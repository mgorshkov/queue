#include <iostream>

#include "QueueStorage.h"

QueueStorage::QueueStorage()
{
}

QueueStorage::~QueueStorage()
{
    Stop();
    mStream.close();
}

void QueueStorage::New(const boost::filesystem::path& aStorageFileName)
{
    mStorageFileName = aStorageFileName;
}

void QueueStorage::Start()
{
    mDone = false;
    mNotified = false;

    mThread = std::move(std::thread(&QueueStorage::ThreadProc, this));
}

void QueueStorage::AddItem(const Item& aItem)
{
    if (mDone.load())
        return;

    std::lock_guard<std::mutex> lk(mStreamMutex);
    mQueue.push(aItem);

    mNotified = true;
    mCondition.notify_one();
}

void QueueStorage::ScheduleShrinkStorage()
{
    AddItem(Item{"", static_cast<std::size_t>(-1)});
}

void QueueStorage::Stop()
{
    mDone = true;
    mNotified = true;
    mCondition.notify_one();
    if (mThread.joinable())
        mThread.join();
    mStream.close();
}

void QueueStorage::ThreadProc()
{
    try
    {
        mStream.open(mStorageFileName.string(), std::fstream::out);
        while (!mDone.load())
        {
            std::unique_lock<std::mutex> lk(mStreamMutex);
            while (!mNotified.load())
                mCondition.wait(lk);
            lk.unlock();
            ProcessQueue();
            mNotified = false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void QueueStorage::ProcessQueue()
{
    while (!mQueue.empty())
    {
        auto item = mQueue.front();
        mQueue.pop();

        if (item.mOffset == static_cast<std::size_t>(-1))
            ShrinkStorage();
        else
            mStream << item;
    }
    mStream.flush();

    if (!CheckFreeDiskSpace())
    {
        ShrinkStorage();
        ShrinkQueue();
    }
}

bool QueueStorage::CheckFreeDiskSpace()
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

void QueueStorage::ShrinkStorage()
{
}

void QueueStorage::ShrinkQueue()
{
}
