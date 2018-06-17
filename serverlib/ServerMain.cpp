#include <boost/asio.hpp>
#include <iostream>

#include "Server.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: queue_server <port>" << std::endl;
            return 1;
        }

        int port = std::atoi(argv[1]);
        if (port == 0)
        {
            std::cerr << "Incorrect port:" << argv[1] << std::endl;
            return 1;
        }

        ba::ip::tcp::endpoint endPoint(ba::ip::tcp::v4(), port);
        ba::io_service ioService;

        Server server(ioService, endPoint);
        server.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
