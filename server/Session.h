#pragma once

#include <array>
#include <boost/asio.hpp>
#include <memory>

#include "structs.h"
#include "context.h"

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
    void Deliver();

    std::shared_ptr<CommandExecutor> mCommandExecutor;
    boost::asio::io_service& mIoService;
    Context mContext;
    boost::asio::ip::tcp::socket mSocket;
    boost::asio::streambuf mBuffer;
    std::deque<std::string> mWriteMsgs;
    std::thread mWriteThread;
    std::atomic_bool mDone{false};
};
