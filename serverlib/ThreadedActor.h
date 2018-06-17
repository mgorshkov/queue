#pragma once

#include <string>
#include <thread>
#include <vector>

class ThreadedActor
{
public:
    ThreadedActor(const std::string& aThreadPrefix, int aThreadCount = 2);

    virtual ~ThreadedActor();

    void Start();
    void Join();

protected:
    virtual void Run() = 0;

private:
    static void ThreadProc(ThreadedActor* aThis, const std::string& aThreadPrefix);

    std::string mThreadPrefix;
    int mThreadCount;
    bool mIsRunning;

    std::vector<std::thread> mThreads;
};

