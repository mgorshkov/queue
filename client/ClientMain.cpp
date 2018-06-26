#include <iostream>
#include <sstream>

#include "Client.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 6 && argc != 5)
        {
            std::cerr << "Usage: queue_client <host> <port> <consumer|producer> <sync|async> <number_of_strings=100>|<start_offset=0>" << std::endl;
            std::cerr << "Ex.: queue_client localhost 9000 producer sync 100" << std::endl;
            std::cerr << "queue_client localhost 9000 consumer async 20" << std::endl;
            return 1;
        }

        std::string host = argv[1];
        int port = atoi(argv[2]);

        std::string consumerProducerModeStr = argv[3];
        if (consumerProducerModeStr != "producer" && consumerProducerModeStr != "consumer")
        {
            std::cerr << "Unknown mode: " << consumerProducerModeStr << std::endl;
            return 1;
        }
        std::string syncAsyncModeStr = argv[4];
        if (syncAsyncModeStr != "sync" && syncAsyncModeStr != "async")
        {
            std::cerr << "Unknown sync/async mode: " << syncAsyncModeStr << std::endl;
            return 1;
        }
        if (consumerProducerModeStr == "producer" && syncAsyncModeStr == "async")
        {
            std::cerr << "Async mode for producer is not supported." << std::endl;
            return 1;
        }

        int number = 100;
        std::size_t startOffset = 0;
        if (argc == 6)
        {
            if (consumerProducerModeStr == "producer")
                number = atoi(argv[5]);
            else
                startOffset = atoll(argv[5]);
        }

        ServerData serverData{host, port};

        ConsumerProducerMode consumerProducerMode;
        std::stringstream consumerProducerModeStream(consumerProducerModeStr);
        consumerProducerModeStream >> consumerProducerMode;

        SyncAsyncMode syncAsyncMode;
        std::stringstream syncAsyncModeStream(syncAsyncModeStr);
        syncAsyncModeStream >> syncAsyncMode;

        Client client(serverData, consumerProducerMode, syncAsyncMode, number, startOffset);
        client.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
