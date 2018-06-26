#pragma once

#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>

class Worker
{
public:
    Worker();
    ~Worker();

    void Stop();

    using ThreadProcType = std::function<void()>;

    void AppendThreadProc(ThreadProcType aThreadProc);
    std::size_t GetTaskCount() const;
    bool IsEmpty() const;

private:
    std::atomic_bool            mDone;
    std::condition_variable     mConditionVariable;
    std::queue<ThreadProcType>  mQueue;
    mutable std::mutex          mMutex;
    std::thread                 mThread; 

    void ThreadProc();
};

using WorkerPtr = std::shared_ptr<Worker>;
