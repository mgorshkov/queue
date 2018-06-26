#pragma once

#include <vector>

#include "Worker.h"

class ThreadPool
{
public:
    ThreadPool(std::size_t aThreads);
    ~ThreadPool();

    template<typename Func, typename... Args>
    void RunAsync(Func aFunc, Args... aArgs)
    {
        GetFreeWorker()->AppendThreadProc(std::bind(aFunc, aArgs...));
    }

private:
    WorkerPtr GetFreeWorker() const;

    std::vector<WorkerPtr> mWorkers; 
};

