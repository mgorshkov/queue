#include "Worker.h"

void Worker::ThreadProc()
{
    while (enabled)
    {
        std::unique_lock<std::mutex> locker(mutex);
        cv.wait(locker, [&]()
        {
            return !mQueue.empty() || !enabled;
        });
        while(!mQueue.empty())
        {
            fn_type func = mQueue.front();
            locker.unlock();
            func();
            locker.lock();
            mQueue.pop();
        }
    }
}