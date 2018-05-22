#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "structs.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(boost::asio::io_service& aIoService, tcp::endpoint aEndPoint, const std::string& aMode);

    void Run();

private:
    void DoConnect(tcp::endpoint aEndPoint);
    void Write(const std::string& aMsg);
    void Read();

private:
    boost::asio::io_service& mIoService;
    tcp::socket mSocket;
    std::string mMode;
};

