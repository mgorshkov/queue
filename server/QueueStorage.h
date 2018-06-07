#pragma once

#include "Defines.h"

#include <thread>
#include <queue>
#include <fstream>
#include <mutex>
#include <condition_variable>

#include <boost/filesystem.hpp>

class QueueStorage
{
public:
    QueueStorage();
    ~QueueStorage();

    void New(const boost::filesystem::path& aStorageFileName);

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

    std::queue<Item> mQueue;

    std::fstream mStream;

    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};

    std::thread mThread;

    boost::filesystem::path mStorageFileName;

    static const uintmax_t FreeSpaceThreshold = 1000;
};

