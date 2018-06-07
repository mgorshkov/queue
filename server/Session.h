#pragma once

#include <array>
#include <memory>

#include "Defines.h"
#include "Context.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket aSocket,
        std::shared_ptr<CommandExecutor> aCommandExecutor,
        boost::asio::io_service& aIoService);
    ~Session();

    void Start();

private:
    void Stop();

    void ProcessWrite();
    void DoRead();
    bool GetWriteQueue();
    void DoWrite();
    void Deliver(std::size_t aLength);

    std::shared_ptr<CommandExecutor> mCommandExecutor;
    boost::asio::io_service& mIoService;
    Context mContext;
    boost::asio::ip::tcp::socket mSocket;
    std::array<char, 256> mBuffer;
    std::deque<std::string> mWriteMsgs;
    std::thread mWriteThread;
    std::atomic_bool mDone{false};
};
