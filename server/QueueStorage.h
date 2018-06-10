#pragma once

#include "Defines.h"
#include "IShrink.h"

#include <thread>
#include <queue>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <boost/filesystem.hpp>

class QueueStorage
{
public:
    QueueStorage(const boost::filesystem::path& aStorageFileName, IShrink* aShrink);
    ~QueueStorage();

    void Start();
    void Stop();

    void AddItem(const Item& aItem);
    void ScheduleShrinkStorage();

private:
    void ThreadProc();
    void ProcessQueue();
    bool CheckFreeDiskSpace();
    void ShrinkStorage();
    void ShrinkQueue();

    IShrink* mShrink;

    std::queue<Item> mQueue;

    std::fstream mStream;

    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};

    std::thread mThread;

    boost::filesystem::path mStorageFileName;

    static const uintmax_t FreeSpaceThreshold = 1000 * 1024;
};

