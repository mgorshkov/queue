#include <future>
#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include "Server.h"
#include "Api.h"

/**
 * @brief Unit tests for queue project.
 */
BOOST_AUTO_TEST_SUITE(queue_integration_test)

struct Fixture
{
    Fixture()
        : mPort(9000)
        , mEndPoint(ba::ip::tcp::v4(), mPort)
        , mServer(mIoService, mEndPoint)
    {
    }

    ~Fixture()
    {
        boost::filesystem::path storageFileNameData("storage/TestQueue/0000000000000000.data");
        boost::filesystem::path storageFileNameIndex("storage/TestQueue/0000000000000000.index");

        boost::filesystem::remove(storageFileNameData);
        boost::filesystem::remove(storageFileNameIndex);

        boost::filesystem::remove("storage/TestQueue");
        boost::filesystem::remove("storage");
    }

    int mPort;
    ba::ip::tcp::endpoint mEndPoint;
    ba::io_service mIoService;

    Server mServer;
};

void RunServer(Fixture* aFixture)
{
    aFixture->mServer.Run();
}

void RunProducer(Fixture* aFixture)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    using namespace QueueApiProducerSync;

    ServerData serverData{"127.0.0.1", aFixture->mPort};
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
}

void RunSyncConsumer(Fixture* aFixture)
{
    using namespace QueueApiConsumerSync;

    ServerData serverData{"127.0.0.1", aFixture->mPort};
    char* errorMessage;
    auto handle = Connect(serverData.mHost.c_str(), serverData.mPort, &errorMessage);

    BOOST_CHECK(handle);

    char* queueList;
    std::size_t queueListLength = GetQueueList(handle, &queueList);

    BOOST_CHECK_EQUAL(queueListLength, 10);
    BOOST_CHECK(!strcmp(queueList, "TestQueue"));

    auto CheckQueue = [handle](const std::string& aQueueName)
    {
        StartQueueSession(handle, aQueueName.c_str());

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

        BOOST_CHECK_EQUAL(i, 100);
    };

    for (char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
    {
        std::string queueName(ptr);
        CheckQueue(queueName);
    }

    delete [] queueList;

    Disconnect(handle);
}

void GetItemsFromQueue(Handle aHandle, const std::string& aQueueName, int i)
{
    using namespace QueueApiConsumerAsync;

    auto dequeueCallback = [aHandle, aQueueName, i](const char* str, std::size_t offset)
    {
        if (offset != -1)
        {
            auto sample = std::string("string") + std::to_string(i);

            BOOST_CHECK_EQUAL(offset, i);
            BOOST_CHECK_EQUAL(sample, str);

            delete [] str;

            GetItemsFromQueue(aHandle, aQueueName, i + 1);
        }
        else
            Disconnect(aHandle);
    };

    Dequeue(aHandle, dequeueCallback);
}

void RunAsyncConsumer(Fixture* aFixture)
{
    using namespace QueueApiConsumerAsync;

    ServerData serverData{"127.0.0.1", aFixture->mPort};

    Handle handle = Connect(serverData.mHost.c_str(), serverData.mPort,
        [&handle](bool, char*)
        {
            GetQueueList(handle,
                [handle](const char* queueList, std::size_t queueListLength)
                {
                    BOOST_CHECK_EQUAL(queueListLength, 10);
                    BOOST_CHECK(!strcmp(queueList, "TestQueue"));

                    for (const char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
                    {
                        std::string queueName(ptr);

                        StartQueueSession(handle,
                            [handle, queueName]
                            {
                                GetItemsFromQueue(handle, queueName, 0);
                            }, queueName.c_str());
                    }
                    delete [] queueList;
                });
        });

    Run(handle);
}

BOOST_FIXTURE_TEST_CASE(test_integration, Fixture)
{
    // server
    std::thread threadServer(&RunServer, this);

    // producer
    std::thread threadProducer(&RunProducer, this);

    threadProducer.join();

    // Sync consumer
    std::thread threadSyncConsumer(&RunSyncConsumer, this);
    threadSyncConsumer.join();

    // Async consumer
    std::thread threadAsyncConsumer(&RunAsyncConsumer, this);
    threadAsyncConsumer.join();

    mServer.Stop();
    threadServer.join();
}

}
