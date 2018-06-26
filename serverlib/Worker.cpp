#include <iostream>

#include "Worker.h"

Worker::Worker()
    : mDone(false)
    , mThread(&Worker::ThreadProc, this)
{
}

Worker::~Worker()
{
    if (!mDone)
        Stop();
}

void Worker::Stop()
{
    mDone = true;
    mConditionVariable.notify_one();
    mThread.join();
}

void Worker::AppendThreadProc(ThreadProcType aThreadProc)
{
    std::unique_lock<std::mutex> locker(mMutex);
    mQueue.push(aThreadProc);
    mConditionVariable.notify_one();
}

std::size_t Worker::GetTaskCount() const
{ 
    std::unique_lock<std::mutex> locker(mMutex);
    return mQueue.size();
}

bool Worker::IsEmpty() const
{ 
    std::unique_lock<std::mutex> locker(mMutex);
    return mQueue.empty();
}

void Worker::ThreadProc()
{
    try
    {
        while (!mDone.load())
        {
            std::unique_lock<std::mutex> locker(mMutex);
            mConditionVariable.wait(locker, [&]()
                {
                    return !mQueue.empty() || mDone.load();
                });
            while (!mQueue.empty())
            {
                auto func = mQueue.front();
                locker.unlock();
                func();
                locker.lock();
                mQueue.pop();
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
