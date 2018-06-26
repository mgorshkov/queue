#include "ThreadPool.h"

WorkerPtr ThreadPool::GetFreeWorker()
{
    WorkerPtr freeWorker;
    size_t minTasks = std::numeric_limits<std::size_t>::max();
    for (const auto& worker : mWorkers)
    {
        if (worker->isEmpty())
            return worker;
        else if (minTasks > worker->GetTaskCount())
        {
            minTasks = worker->GetTaskCount();
            freeWorker = it;
        }
    }
    return freeWorker;
}
