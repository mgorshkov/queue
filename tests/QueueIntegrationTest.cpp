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

    ba::ip::tcp::endpoint endPoint(ba::ip::tcp::v4(), port);
    ba::io_service ioService;

    boost::filesystem::path storageFileNameData("storage/TestQueue/0000000000000000.data");
    boost::filesystem::path storageFileNameIndex("storage/TestQueue/0000000000000000.index");

    boost::filesystem::remove(storageFileNameData);
    boost::filesystem::remove(storageFileNameIndex);

    boost::filesystem::remove("storage/TestQueue");
    boost::filesystem::remove("storage");

    Server server(ioService, endPoint);

    // server
    std::thread threadServer([&server]()
        {
            server.Run();
        });

    // producer
    std::thread threadProducer([port]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            using namespace QueueApiProducerSync;

            ServerData serverData{"127.0.0.1", port};
            char* errorMessage;
            auto handle = Connect(serverData.mHost.c_str(), serverData.mPort, &errorMessage);

            BOOST_CHECK(handle);

            StartQueueSession(handle, "TestQueue");

            for (int i = 0; i < 100; ++i)
            {
                auto str = std::string("string") + std::to_string(i);
                Enqueue(handle, str.c_str());
            }

            Disconnect(handle);
       });

    // Sync consumer
    std::thread threadSyncConsumer([port]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            using namespace QueueApiConsumerSync;

            ServerData serverData{"127.0.0.1", port};
            char* errorMessage;
            auto handle = Connect(serverData.mHost.c_str(), serverData.mPort, &errorMessage);

            BOOST_CHECK(handle);

            char* queueList;
            std::size_t queueListLength = GetQueueList(handle, &queueList);

            BOOST_CHECK_EQUAL(queueListLength, 10);
            BOOST_CHECK(!strcmp(queueList, "TestQueue"));

            for (char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
            {
                std::string queueName(ptr);

                StartQueueSession(handle, ptr);

                int i = 0;
                while (true)
                {
                    char* str;
                    std::size_t offset;
                    if (!Dequeue(handle, &str, &offset))
                        break;
                    auto sample = std::string("string") + std::to_string(i);

                    BOOST_CHECK_EQUAL(offset, i);
                    BOOST_CHECK_EQUAL(sample, str);

                    delete [] str;

                    i++;
                }
            }

            delete [] queueList;

            Disconnect(handle);
       });

    // Async consumer
    std::thread threadAsyncConsumer([port]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            using namespace QueueApiConsumerAsync;

            ServerData serverData{"127.0.0.1", port};

            Handle handle = Connect(serverData.mHost.c_str(), serverData.mPort,
                [&handle](bool ok, char* errorMessage)
                {
                    GetQueueList(handle,
                        [handle](const char* queueList, std::size_t queueListLength)
                        {
                            BOOST_CHECK_EQUAL(queueListLength, 10);
                            BOOST_CHECK(!strcmp(queueList, "TestQueue"));
                            
                            for (const char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
                            {
                                StartQueueSession(handle,
                                    [handle, ptr]
                                    {
                                        Dequeue(handle, 
                                            [handle](const char* str, std::size_t offset)
                                            {
                                                int i = 0;
                                                auto sample = std::string("string") + std::to_string(i);

                                                BOOST_CHECK_EQUAL(offset, i);
                                                BOOST_CHECK_EQUAL(sample, str);

                                                delete [] str;

                                                Disconnect(handle);

                                            });
                                    }, ptr);
                            }
                        });
                });
            Run(handle);
       });

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    server.Stop();
    threadProducer.join();
    threadSyncConsumer.join();
    threadAsyncConsumer.join();
    threadServer.join();
}

}
