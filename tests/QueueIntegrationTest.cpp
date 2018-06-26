#include <future>
#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include "Server.h"
#include "Api.h"

/**
 * @brief Unit tests for queue project.
 */
BOOST_AUTO_TEST_SUITE(queue_integration_test)

BOOST_AUTO_TEST_CASE(test_integration)
{
    namespace ba = boost::asio;

    const int port = 9000;

    // server
    std::async(std::launch::async, [port]()
        {
            ba::ip::tcp::endpoint endPoint(ba::ip::tcp::v4(), port);
            ba::io_service ioService;

            Server server(ioService, endPoint);
            server.Run();
        });

    // producer
    std::async(std::launch::async, [port]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            using namespace QueueApiProducerSync;

            ServerData serverData{"127.0.0.1", port};
            char* errorMessage;
            auto handle = Connect(serverData.mHost.c_str(), serverData.mPort, &errorMessage);

            BOOST_CHECK(handle);

            StartQueueSession(handle, "TestQueue");

            for (int i = 0; i < 100; ++i)
                Enqueue(handle, (std::string("string") + std::to_string(i)).c_str());

            Disconnect(handle);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
       });

    // Sync consumer
    std::async(std::launch::async, [port]()
        {
            using namespace QueueApiConsumerSync;

            ServerData serverData{"127.0.0.1", port};
            char* errorMessage;
            auto handle = Connect(serverData.mHost.c_str(), serverData.mPort, &errorMessage);

            BOOST_CHECK(handle);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            char* queueList;
            std::size_t queueListLength = GetQueueList(handle, &queueList);
            std::cout << queueListLength << std::endl;
            std::cout << "Queues:" << std::endl;
            for (char* ptr = queueList; queueList + queueListLength <= ptr; ptr += strlen(ptr) + 1)
            {
                std::string queueName(ptr);
                std::cout << queueName << std::endl;

                StartQueueSession(handle, ptr);

                char* str;
                std::size_t offset;
                Dequeue(handle, &str, &offset);

                std::cout << "Item:" << str << ", offset:" << offset << std::endl;
            }

            Disconnect(handle);
       });

    // Async consumer
    std::async(std::launch::async, [port]()
        {
            using namespace QueueApiConsumerAsync;

            ServerData serverData{"127.0.0.1", port};

            Handle handle = Connect(serverData.mHost.c_str(), serverData.mPort,
                [&handle](bool ok, char* errorMessage)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    GetQueueList(handle,
                        [handle](const char* queueList, std::size_t queueListLength)
                        {
                            std::cout << queueListLength << std::endl;
                            std::cout << "Queues:" << std::endl;
                            for (const char* ptr = queueList; queueList + queueListLength <= ptr; ptr += strlen(ptr) + 1)
                            {
                                std::string queueName(ptr);
                                std::cout << queueName << std::endl;

                                StartQueueSession(handle,
                                    [handle, ptr]
                                    {
                                        Dequeue(handle, 
                                            [handle](const char* str, std::size_t offset)
                                            {
                                                std::cout << "Item:" << str << ", offset:" << offset << std::endl;

                                                Disconnect(handle);

                                            });
                                    }, ptr, -1);
                            }
                        });
                });
            Run(handle);
       });

}

}
