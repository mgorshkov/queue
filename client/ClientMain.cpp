#include <boost/asio.hpp>

#include "client.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: queue_client <host> <port> <mode>" << std::endl;
            return 1;
        }

        boost::asio::io_service ioService;

        auto host = argv[1];
        int port = std::atoi(argv[2]);
        if (port == 0)
        {
            std::cerr << "Incorrect port: " << port << std::endl;
            return 1;
        }
        auto mode = std::atoi(argv[3]);
        if (mode != "producer" && mode != "consumer")
        {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return 1;
        }

        boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::address::from_string(host), port);
        Client client(ioService, endPoint, mode);
        client.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
