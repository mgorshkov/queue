#include "ThreadPool.h"

ThreadPool::ThreadPool(std::size_t aThreads)
{
    for (std::size_t i = 0; i < aThreads; ++i)
    {
        mWorkers.push_back(std::make_shared<Worker>());
    }
}

ThreadPool::~ThreadPool()
{
}

WorkerPtr ThreadPool::GetFreeWorker() const
{
    WorkerPtr freeWorker;
    size_t minTasks = std::numeric_limits<std::size_t>::max();
    for (const auto& worker : mWorkers)
    {
        if (worker->IsEmpty())
            return worker;
        else if (minTasks > worker->GetTaskCount())
        {
            minTasks = worker->GetTaskCount();
            freeWorker = worker;
        }
    }
    return freeWorker;
}
