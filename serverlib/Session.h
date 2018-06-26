#pragma once

#include <array>
#include <memory>

#include <boost/asio.hpp>

#include "Defines.h"
#include "Context.h"

namespace ba = boost::asio;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(ThreadPool& aThreadPool, ba::ip::tcp::socket aSocket,
        CommandExecutorPtr aCommandExecutor,
        ba::io_service& aIoService);
    ~Session();

    void Start();

private:
    void Stop();

    void ProcessWrite();
    void DoRead();
    bool GetWriteQueue();
    void DoWrite();
    void Deliver(std::size_t aLength);

    CommandExecutorPtr mCommandExecutor;
    ba::io_service& mIoService;
    Context mContext;
    ba::ip::tcp::socket mSocket;
    BufferType mBuffer;
    std::deque<std::string> mWriteMsgs;
    ThreadPool& mThreadPool;
    std::atomic_bool mDone{false};
};
