#include <iostream>
#include <assert.h>
#include <string.h>

#include "Client.h"

Client::Client(const ServerData& aServerData,
    ConsumerProducerMode aConsumerProducerMode,
    SyncAsyncMode aSyncAsyncMode,
    int aNumber,
    std::size_t aStartOffset)
    : mServerData(aServerData)
    , mConsumerProducerMode(aConsumerProducerMode)
    , mSyncAsyncMode(aSyncAsyncMode)
    , mNumber(aNumber)
    , mStartOffset(aStartOffset)
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
    std::cout << "Sync consumer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "...";

    char* errorMessage;
    auto handle = QueueApiConsumerSync::Connect(mServerData.mHost.c_str(), mServerData.mPort, &errorMessage);

    if (!handle)
    {
        std::cout << "error: " << errorMessage << std::endl;
        delete [] errorMessage;
        return;
    }

    std::cout << "success" << std::endl;

    std::cout << "Getting list of queues...";
    char* queueList;
    std::size_t queueListLength = QueueApiConsumerSync::GetQueueList(handle, &queueList);
    std::cout << "ok" << std::endl;
#ifdef DEBUG_PRINT
    std::cout << queueListLength << std::endl;
#endif
    std::cout << "Queues:" << std::endl;
    if (queueListLength == 0)
        std::cout << "empty." << std::endl;
    else
    {
        for (char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
        {
            std::string queueName(ptr);
            std::cout << queueName << std::endl;
        }
        for (char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
        {
            std::string queueName(ptr);

            std::cout << "Starting session with queue " << queueName << "...";
            QueueApiConsumerSync::StartQueueSession(handle, ptr, mStartOffset);
            std::cout << "ok" << std::endl;

            std::cout << "Getting items from queue " << queueName << "..." << std::endl;
            while (true)
            {
                char* str;
                std::size_t offset;
                if (!QueueApiConsumerSync::Dequeue(handle, &str, &offset))
                {
                    std::cout << "error" << std::endl;
                    break;
                }

                std::cout << "Item:";
                std::cout << str << ", offset:" << offset << " " << "ok" << std::endl;

                delete [] str;
            }
        }
    }
    delete [] queueList;
}

void Client::RunConsumerAsync()
{
    std::cout << "Async consumer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "...";

    Handle handle = 0;
    auto connectCallback = [&handle](bool ok, char* errorMessage)
    {
        if (!ok)
        {
            std::cout << "error: " << errorMessage << std::endl;
            delete [] errorMessage;
            return;
        }

        std::cout << "success." << std::endl;

        auto queueListCallback = [handle](const char* queueList, std::size_t queueListLength)
        {
            std::cout << "Queues:" << std::endl;
            for (const char* ptr = queueList; ptr < queueList + queueListLength; ptr += strlen(ptr) + 1)
            {
                std::string queueName(ptr);
                std::cout << queueName << std::endl;

                auto queueSessionCallback = [handle]()
                {
                    auto dequeueCallback = [handle](const char* str, std::size_t offset)
                    {
                        std::cout << "Item:" << str << ", offset: " << offset << std::endl;
                    };

                    QueueApiConsumerAsync::Dequeue(handle, dequeueCallback);
                };

                QueueApiConsumerAsync::StartQueueSession(handle, queueSessionCallback, ptr);

                auto dequeueCallback = [](const char* str, std::size_t offset)
                {
                    std::cout << "Item:" << str << ", offset:" << offset << std::endl;
                };

                QueueApiConsumerAsync::Dequeue(handle, dequeueCallback);
            }
        };

        QueueApiConsumerAsync::GetQueueList(handle, queueListCallback);
    };

    handle = QueueApiConsumerAsync::Connect(mServerData.mHost.c_str(), mServerData.mPort, connectCallback);
    QueueApiConsumerAsync::Run(handle);
}

void Client::RunProducerSync()
{
    std::cout << "Sync producer client started." << std::endl;

    std::cout << "Connecting to " << mServerData.mHost << ":" << mServerData.mPort << "..." << std::endl;

    char* errorMessage;
    auto handle = QueueApiProducerSync::Connect(mServerData.mHost.c_str(), mServerData.mPort, &errorMessage);

    if (!handle)
    {
        std::cout << "Error: " << errorMessage << std::endl;
        delete [] errorMessage;
        return;
    }

    std::cout << "Success." << std::endl;

    QueueApiProducerSync::StartQueueSession(handle, "TestQueue");

    for (int i = 0; i < mNumber; ++i)
    {
        auto str = std::string("str") + std::to_string(i);
        QueueApiProducerSync::Enqueue(handle, str.c_str());
        std::cout << "Enqueued string " << str << std::endl;
    }

    QueueApiProducerSync::Disconnect(handle);
}
