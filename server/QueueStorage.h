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
    void Load(const boost::filesystem::path& aStorageFileName);

    void Start();
    void Stop();

    void AddItem(const Item& aItem);

private:
    void ThreadProc();
    void ProcessQueue();

    std::queue<Item> mQueue;

    std::fstream mStream;

    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};

    std::thread mThread;
};

