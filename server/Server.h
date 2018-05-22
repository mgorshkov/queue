#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "tablemanager.h"
#include "commandexecutor.h"

class Server
{
public:
    Server(boost::asio::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndpoint);

private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor mAcceptor;
    boost::asio::ip::tcp::socket mSocket;
    boost::asio::io_service& mIoService;

    TableManager mTableManager;
    std::shared_ptr<CommandExecutor> mCommandExecutor;
};
