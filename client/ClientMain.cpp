#include <iostream>

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
        auto consumerProducerModeStr = argv[3];
        if (consumerProducerModeStr != "producer" && consumerProducerModeStr != "consumer")
        {
            std::cerr << "Unknown mode: " << consumerProducerModeStr << std::endl;
            return 1;
        }
        auto syncAsyncModeStr = argv[4];
        if (syncAsyncModeStr != "sync" && syncAsyncModeStr != "async")
        {
            std::cerr << "Unknown sync/async mode: " << syncAsyncModeStr << std::endl;
        }

        ServerData serverData{host, port};

        ConsumerProducerMode consumerProducerMode;
        std::stringstream consumerProducerModeStream(consumerProducerModeStr);
        consumerProducerModeStream >> consumerProducerMode;

        SyncAsyncMode syncAsyncMode;
        std::stringstream syncAsyncModeStream(syncAsyncModeStr);
        syncAsyncModeStream >> syncAsyncMode;

        Client client(serverData, consumerProducerMode, syncAsyncMode);
        client.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
