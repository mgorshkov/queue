#pragma once

#include <vector>

class ThreadPool
{
public:
    ThreadPool(std::size_t aThreads)
    {
        for (std::size_t i = 0; i < aThreads; ++i)
        {
            mWorkers.push_back(std::make_shared<Worker>());
        }
    }
    ~ThreadPool() {}

private:
    WorkerPtr GetFreeWorker() const;

    std::vector<WorkerPtr> mWorkers; 
};

