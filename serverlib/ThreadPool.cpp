#include "ThreadPool.h"

void ThreadProc()
{
}

ThreadPtr ThreadPool::Get()
{
    for (auto& poolElem : mPool)
    {
        if (!poolElem.mBusy)
        {
            poolElem.mBusy = true;
            return std::move(poolElem.mThread);
        }
    }

    auto thread = std::make_unique<std::thread>(&ThreadProc);

    mPool.emplace_back(std::move(thread), true);

    return std::move(mPool[mPool.size() - 1].mThread);
}

void ThreadPool::Put(ThreadPtr aThread)
{
    for (size_t i = 0; i < mPool.size(); ++i)
    {
        if (mPool[i].mBusy)
        {
            mPool[i].mThread = std::move(aThread);
            mPool[i].mBusy = false;
            break;
        }
    }
}

