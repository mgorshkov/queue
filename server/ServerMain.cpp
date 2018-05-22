#include <boost/asio.hpp>
#include <iostream>

#include "server.h"

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

        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        boost::asio::io_service ioService;

        Server server(ioService, endpoint);

        ioService.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
