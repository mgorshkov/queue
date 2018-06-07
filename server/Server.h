#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "CommandExecutor.h"
#include "QueueManager.h"

class Server
{
public:
    Server(boost::asio::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndpoint);
    void Run();

private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor mAcceptor;
    boost::asio::ip::tcp::socket mSocket;
    boost::asio::io_service& mIoService;

    std::shared_ptr<CommandExecutor> mCommandExecutor;
    QueueManager mQueueManager;
};
