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
    mCommandContext = nullptr;

    mThread = std::move(std::thread(&Context::ThreadProc, this));
}

void Context::ProcessData(const BufferType& aBuffer, std::size_t aLength)
{
#ifdef DEBUG_PRINT
    std::cout << "Context::ProcessData, this==" << this << ", aBuffer=" << aBuffer << ", mDone=" << mDone.load() << std::endl;
#endif
    if (mDone.load())
        return;
    {
        std::lock_guard<std::mutex> lk(mStreamMutex);
#ifdef DEBUG_PRINT
        std::cout << "Context::ProcessData 2, pos = " << mStream.tellp() << std::endl;
#endif
        for (std::size_t i = 0; i < aLength; ++i)
            mStream << aBuffer[i];
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

MessagePtrs Context::GetOutboundQueue()
{
    if (!mQueueNotified.load())
    {
#ifdef DEBUG_PRINT
        std::cout << "Context::GetOutboundQueue 1" << std::endl;
#endif
        return MessagePtrs{};
    }
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 2" << std::endl;
#endif
    mQueueNotified = false;
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 3" << std::endl;
#endif
    if (mOutboundMessages.empty())
        return MessagePtrs{};
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 4" << std::endl;
#endif
    auto statuses = mOutboundMessages.front();
    mOutboundMessages.pop();
#ifdef DEBUG_PRINT
    std::cout << "Context::GetOutboundQueue 5" << std::endl;
    for (auto status : statuses)
        std::cout << status << std::endl;
#endif

    return statuses;
}

MessagePtrs Context::ProcessStream()
{
    std::list<MessagePtr> messages;
    {
        std::lock_guard<std::mutex> lk(mStreamMutex);
        mStream.seekp(0);
        while (mStream)
        {
            auto message = ProtocolSerializer::Deserialize(mStream);
            if (message)
                messages.push_back(message);
        }
        mStream.clear();
        mStream.str("");
    }
    return ProcessMessages(messages);
}

MessagePtrs Context::ProcessMessages(const std::list<MessagePtr> aMessages)
{
    MessagePtrs results;
    for (const auto& message: aMessages)
    {
        auto startSessionMessage = std::dynamic_pointer_cast<StartQueueSessionMessage>(message);
        if (startSessionMessage)
            mCommandContext = std::make_unique<CommandContext>(startSessionMessage);
        else if (mCommandContext)
        {
            CompleteCommand command{message, *mCommandContext};
            auto result = mCommandExecutor->RunCommand(command);
            results.push_back(result);
        }
    }
    return results;
}

void Context::ThreadProc()
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
                mOutboundMessages.push(ProcessStream());
            }
            mQueueCondition.notify_one();
            mNotified = false;
            mQueueNotified = true;
        }
        {
            std::unique_lock<std::mutex> lk(mQueueMutex);
            mOutboundMessages.push(ProcessStream());
        }
        mQueueCondition.notify_one();
        mQueueNotified = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
