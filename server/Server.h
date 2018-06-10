#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "CommandExecutor.h"
#include "QueueManager.h"

namespace ba = boost::asio;

class Server
{
public:
    Server(ba::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndpoint);
    void Run();

private:
    void DoAccept();

    ba::ip::tcp::acceptor mAcceptor;
    ba::ip::tcp::socket mSocket;
    ba::io_service& mIoService;

    std::shared_ptr<CommandExecutor> mCommandExecutor;
    QueueManager mQueueManager;
};
