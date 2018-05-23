#pragma once

#include <boost/asio.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <condition_variable>
#include <atomic>
#include <queue>

#include "commandexecutor.h"
#include "structs.h"

class Context
{
public:
    Context(std::shared_ptr<CommandExecutor> aCommandExecutor = nullptr);
    ~Context();

    void SetExecutor(std::shared_ptr<CommandExecutor> aCommandExecutor);

    void ProcessData(boost::asio::streambuf* aStream);

    void Start();
    void Stop();

    CompleteOperationStatuses GetOutboundQueue();

private:
    CompleteOperationStatuses ProcessStream(std::shared_ptr<CommandExecutor> aCommandExecutor);

    static void ThreadProc(Context* aContext, std::shared_ptr<CommandExecutor> aCommandExecutor);

    std::shared_ptr<CommandExecutor> mCommandExecutor;

    std::stringstream mStream;
    std::mutex mStreamMutex;
    std::condition_variable mCondition;
    std::condition_variable mQueueCondition;
    std::atomic_bool mDone{false};
    std::atomic_bool mNotified{false};
    std::atomic_bool mQueueNotified{false};

    std::thread mThread;

    std::queue<CompleteOperationStatuses> mOutboundStatuses;
    std::mutex mQueueMutex;
};
