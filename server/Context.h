#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <condition_variable>
#include <atomic>
#include <queue>

#include "CommandExecutor.h"
#include "Defines.h"

class Context
{
public:
    Context(std::shared_ptr<CommandExecutor> aCommandExecutor = nullptr);
    ~Context();

    void SetExecutor(std::shared_ptr<CommandExecutor> aCommandExecutor);

    void ProcessData(ba::streambuf* aStream);

    void Start();
    void Stop();

    MessagePtrs GetOutboundQueue();

private:
    MessagePtrs ProcessStream(std::shared_ptr<CommandExecutor> aCommandExecutor);

    void ThreadProc(std::shared_ptr<CommandExecutor> aCommandExecutor);

    std::shared_ptr<CommandExecutor> mCommandExecutor;

    std::stringstream mStream;
    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::condition_variable mQueueCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};
    std::atomic_bool mQueueNotified{false};

    std::thread mThread;

    std::queue<MessagePtrs> mOutboundMessages;
    std::mutex mQueueMutex;
};
