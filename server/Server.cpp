#include "server.h"
#include "session.h"

Server::Server(boost::asio::io_service& aIoService, const boost::asio::ip::tcp::endpoint& aEndPoint)
    : mAcceptor(aIoService, aEndPoint)
    , mSocket(aIoService)
    , mIoService(aIoService)
{
    mCommandExecutor = std::make_shared<CommandExecutor>(&mTableManager);
    DoAccept();
}

void Server::DoAccept()
{
    mAcceptor.async_accept(mSocket,
        [this](boost::system::error_code ec)
        {
            if (ec)
                std::cout << "Accept error: " << ec.message() << std::endl;
            else
                std::make_shared<Session>(std::move(mSocket), mCommandExecutor, mIoService)->Start();

            DoAccept();
        });
}
