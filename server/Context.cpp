#include <iostream>
#include <chrono>

#include "Context.h"
#include "Defines.h"
#include "ProtocolSerializer.h"

using namespace std::chrono_literals;

Context::Context(std::shared_ptr<CommandExecutor> aCommandExecutor)
    : mCommandExecutor(aCommandExecutor)
{
#ifdef DEBUG_PRINT
    std::cout << "Context::Context, this==" << this << std::endl;
#endif
}

Context::~Context()
{
#ifdef DEBUG_PRINT
    std::cout << "Context::~Context, this==" << this << std::endl;
#endif
    Stop();
}

void Context::SetExecutor(std::shared_ptr<CommandExecutor> aCommandExecutor)
{
    mCommandExecutor = aCommandExecutor;
}

void Context::Start()
{
#ifdef DEBUG_PRINT
    std::cout << "Context::Start, this==" << this << std::endl;
#endif

    mDone = false;
    mNotified = false;
    mQueueNotified = false;
    mStream.clear();
    mStream.str("");

    mThread = std::move(std::thread(&Context::ThreadProc, this, mCommandExecutor));
}

void Context::ProcessData(ba::streambuf* aStream)
{
#ifdef DEBUG_PRINT
    std::cout << "Context::ProcessData, this==" << this << ", aStream=" << aStream << ", mDone=" << mDone.load() << std::endl;
#endif
    if (mDone.load())
        return;
    {
        std::lock_guard<std::mutex> lk(mStreamMutex);
#ifdef DEBUG_PRINT
        std::cout << "Context::ProcessData 2, pos = " << mStream.tellp() << std::endl;
#endif
        mStream << aStream;
#ifdef DEBUG_PRINT
        std::cout << "Context::ProcessData 3, pos = " << mStream.tellp() << std::endl;
#endif
    }
    mNotified = true;
    mCondition.notify_one();
#ifdef DEBUG_PRINT
    std::cout << "Context::ProcessData end, this==" << this << ", stream=" << mStream.str() << std::endl;
#endif
}

void Context::Stop()
{
#ifdef DEBUG_PRINT
    std::cout << "Context::Stop1, this==" << this << ", stream=" << mStream.str() << std::endl;
#endif
    mDone = true;
    mNotified = true;
#ifdef DEBUG_PRINT
    std::cout << "Context::Stop2, this==" << this << std::endl;
#endif
    mCondition.notify_one();
#ifdef DEBUG_PRINT
    std::cout << "Context::Stop3, this==" << this << std::endl;
#endif
    if (mThread.joinable())
        mThread.join();
#ifdef DEBUG_PRINT
    std::cout << "Context::Stop4, this==" << this << std::endl;
#endif
}

CompleteOperationStatuses Context::GetOutboundQueue()
{
    if (!mQueueNotified.load())
    {
#ifdef DEBUG_PRINT
        std::cout << "Context::GetOutboundQueue 1" << std::endl;
#endif
        return CompleteOperationStatuses{};
    }
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 2" << std::endl;
#endif
    mQueueNotified = false;
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 3" << std::endl;
#endif
    if (mOutboundStatuses.empty())
        return CompleteOperationStatuses{};
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 4" << std::endl;
#endif
    auto statuses = mOutboundStatuses.front();
    mOutboundStatuses.pop();
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 5" << std::endl;
    for (auto status : statuses)
        std::cout << status << std::endl;
#endif

    return statuses;
}

CompleteOperationStatuses Context::ProcessStream(std::shared_ptr<CommandExecutor> aCommandExecutor)
{
    std::list<MessagePtr> messages;
    {
        std::lock_guard<std::mutex> lk(mStreamMutex);
        mStream.seekp(0);
        while (mStream)
        {
            auto message = ProtocolSerializer::Deserialize(mStream);
            messages.push_back(message);
        }
        mStream.clear();
        mStream.str("");
    }
    CompleteOperationStatuses results;
    for (const auto& message: messages)
    {
        CompleteCommand command{message};
        auto result = aCommandExecutor->RunCommand(command);
        results.push_back(result);
    }
    return results;
}

void Context::ThreadProc(std::shared_ptr<CommandExecutor> aCommandExecutor)
{
    try
    {
        while (!mDone.load())
        {
            std::unique_lock<std::mutex> lk(mStreamMutex);
            while (!mNotified.load())
                mCondition.wait(lk);
            lk.unlock();
            {
                std::unique_lock<std::mutex> lk(mQueueMutex);
                mOutboundStatuses.push(ProcessStream(aCommandExecutor));
            }
            mQueueCondition.notify_one();
            mNotified = false;
            mQueueNotified = true;
        }
        {
            std::unique_lock<std::mutex> lk(mQueueMutex);
            mOutboundStatuses.push(ProcessStream(aCommandExecutor));
        }
        mQueueCondition.notify_one();
        mQueueNotified = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
