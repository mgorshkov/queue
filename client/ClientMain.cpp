#include "Client.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: queue_client <host> <port> <consumer|producer> <sync|async>" << std::endl << "Ex.: queue_client localhost 9000 consumer async" << std::endl;
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
        auto actorMode = argv[3];
        if (actorMode != "producer" && actorMode != "consumer")
        {
            std::cerr << "Unknown mode: " << actorMode << std::endl;
            return 1;
        }
        auto syncAsyncMode = argv[4];
        if (syncAsyncMode != "sync" && syncAsyncMode != "async")
        {
            std::cerr << "Unknown sync/async mode: " << syncAsyncMode << std::endl;
        }

        ServerData serverData{host, port};
        Client client(serverData, actorMode, syncAsyncMode);
        client.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
