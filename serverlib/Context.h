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
    Context(CommandExecutorPtr aCommandExecutor = nullptr);
    ~Context();

    void SetExecutor(CommandExecutorPtr aCommandExecutor);

    void ProcessData(const BufferType& aBuffer, std::size_t aLength);

    void Start();
    void Stop();

    MessagePtrs GetOutboundQueue();

private:
    MessagePtrs ProcessStream();
    MessagePtrs ProcessMessages(const std::list<MessagePtr> aMessages);

    void ThreadProc();

    CommandExecutorPtr mCommandExecutor;
    CommandContextPtr mCommandContext;

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
