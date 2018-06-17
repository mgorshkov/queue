#include <iostream>
#include <assert.h>
#include <string.h>

#include "Client.h"

Client::Client(const ServerData& aServerData, ConsumerProducerMode aConsumerProducerMode, SyncAsyncMode aSyncAsyncMode)
    : mServerData(aServerData)
    , mConsumerProducerMode(aConsumerProducerMode)
    , mSyncAsyncMode(aSyncAsyncMode)
{
}

void Client::Run()
{
    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::Consumer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::Sync:
            RunConsumerSync();
            break;

        case SyncAsyncMode::Async:
            RunConsumerAsync();
            break;

        default:
            assert(0);
        }
        break;
    case ConsumerProducerMode::Producer:
        RunProducerSync();
        break;

    default:
        assert(0);
    }
}

void Client::RunConsumerSync()
{
    using namespace QueueApiSync;

    std::cout << "Sync consumer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "..." << std::endl;

    char* errorMessage;
    auto handle = Connect(mServerData.mHost.c_str(), mServerData.mPort, &errorMessage, false);

    if (!handle)
    {
        std::cout << "Error: " << errorMessage << std::endl;
        delete [] errorMessage;
        return;
    }

    std::cout << "Success." << std::endl;

    char* queueList;
    std::size_t queueListLength = GetQueueList(handle, &queueList);

    std::cout << "Queues:" << std::endl;
    for (char* ptr = queueList; queueList + queueListLength <= ptr; ptr += strlen(ptr) + 1)
    {
        std::string queueName(ptr);
        std::cout << queueName << std::endl;

        StartQueueSession(handle, ptr);

        char* str;
        std::size_t* offset;
        Dequeue(handle, &str, &offset);
        
        std::cout << "Item:" << str << ", offset:" << *offset << std::endl;
    }
}

void Client::RunConsumerAsync()
{
    using namespace QueueApiAsync;

    std::cout << "Async consumer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "..." << std::endl;

    auto connectCallback = [](Handle handle, char* errorMessage)
    {
        if (!handle)
        {
            std::cout << "Error: " << errorMessage << std::endl;
            delete [] errorMessage;
            return;
        }

        std::cout << "Success." << std::endl;

        auto queueListCallback = [handle](const char* queueList, std::size_t queueListLength)
        {
            std::cout << "Queues:" << std::endl;
            for (const char* ptr = queueList; queueList + queueListLength <= ptr; ptr += strlen(ptr) + 1)
            {
                std::string queueName(ptr);
                std::cout << queueName << std::endl;

                auto queueSessionCallback = [handle]()
                {
                    auto dequeueCallback = [handle](const char* str, std::size_t offset)
                    {
                        std::cout << "Item:" << str << ", offset: " << offset << std::endl;
                    };
                    
                    Dequeue(handle, dequeueCallback);
                };

                StartQueueSession(handle, queueSessionCallback, ptr);

                auto dequeueCallback = [](const char* str, std::size_t offset)
                {
                    std::cout << "Item:" << str << ", offset:" << offset << std::endl;
                };

                Dequeue(handle, dequeueCallback);        
            }
        };

        GetQueueList(handle, queueListCallback);
    };

    Connect(mServerData.mHost.c_str(), mServerData.mPort, connectCallback);
}

void Client::RunProducerSync()
{
    using namespace QueueApiSync;

    std::cout << "Sync producer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "..." << std::endl;
                    
    char* errorMessage;
    auto handle = Connect(mServerData.mHost.c_str(), mServerData.mPort, &errorMessage, true);

    if (!handle)
    {
        std::cout << "Error: " << errorMessage << std::endl;
        delete [] errorMessage;
        return;
    }

    std::cout << "Success." << std::endl;
    
    StartQueueSession(handle, "TestQueue");

    Enqueue(handle, "str1");
    Enqueue(handle, "str2");
    Enqueue(handle, "str3");

    Disconnect(handle);
}
