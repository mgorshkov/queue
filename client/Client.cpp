#include <iostream>

#include "client.h"

using boost::asio::ip::tcp;

Client::Client(boost::asio::io_service& aIoService, tcp::endpoint aEndPoint, const std::string& aMode)
    : mIoService(aIoService)
    , mSocket(aIoService)
    , mMode(aMode)
{
    DoConnect(aEndPoint);
}

void Client::Run()
{
    Write("INSERT A 0 lean\n");
    Write("INSERT A 0 understand\n");
    Write("INSERT A 1 sweater\n");
    Write("INSERT A 2 frank\n");
    Write("INSERT B 0 flour\n");
    Write("INSERT B 2 wonder\n");
    Write("INSERT B 2 selection\n");
    Write("INTERSECTION\n");
    Read();
    mSocket.close();
}

void Client::Write(const std::string& aMsg)
{
    boost::asio::write(mSocket, boost::asio::buffer(aMsg.c_str(), aMsg.length()));
}

void Client::DoConnect(tcp::endpoint aEndPoint)
{
    mSocket.connect(aEndPoint);
}

void Client::Read()
{
    char data[256];
    std::cout << "Client::Read" << std::endl;
    size_t len = mSocket.read_some(boost::asio::buffer(data));
    std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
}
