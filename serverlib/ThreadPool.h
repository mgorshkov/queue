#pragma once

#include <memory>
#include <thread>
#include <vector>

using ThreadPtr = std::unique_ptr<std::thread>;

class ThreadPool
{
public:
    ThreadPtr Get();

    struct ThreadInfo
    {
        ThreadInfo(ThreadPtr aThread, bool aBusy)
            : mThread(std::move(aThread))
            , mBusy(aBusy)
        {}
        ThreadPtr mThread;
        bool mBusy;
    };

    void Put(ThreadPtr aThread);

private:
    std::vector<ThreadInfo> mPool;
};

