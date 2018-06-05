#include <iostream>

#include "QueueStorage.h"

QueueStorage::QueueStorage()
{
}

QueueStorage::~QueueStorage()
{
    Stop();
}

void QueueStorage::Load(const boost::filesystem::path& aStorageFileName)
{
    mStream.open(aStorageFileName.string());
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

void QueueStorage::Stop()
{
    mDone = true;
    mNotified = true;
    mCondition.notify_one();
    if (mThread.joinable())
        mThread.join();
}

void QueueStorage::ThreadProc()
{
    try
    {
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
    auto item = mQueue.front();
    mQueue.pop();
    mStream << item;
}
