#include <iostream>

#include "Client.h"
#include "ConsumerApiClient.h"
#include "ProducerApiClient.h"

Client::Client(const ServerData& aServerData, ConsumerProducerMode aConsumerProducerMode, SyncAsyncMode aSyncAsyncMode)
    : mServerData(aServerData)
    , mConsumerProducerMode(aConsumerProducerMode)
    , mSyncAsyncMode(aSyncAsyncMode)
{
}

void Client::CreateApiClient()
{
    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::Consumer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::Sync:
            mApiClient = std::make_unique<ConsumerApiClientSync>(mIoService);
            break;
        case SyncAsyncMode::Async:
            mApiClient = std::make_unique<ConsumerApiClientAsync>(mIoService);
            break;
        }
        break;
    case ConsumerProducerMode::Producer:
        mApiClient = std::make_unique<ProducerApiClient>(mIoService);
        break;
    }
}

void Client::Run()
{
    CreateApiClient();

    std::cout << "Client started." << std::endl;

    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::Consumer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::Sync:
            {
                auto client = dynamic_cast<ConsumerApiClientSync*>(mApiClient.get());
                QueueList queueList = client->GetQueueList();
                std::cout << "Queues:" << std::endl;
                for (auto queueName : queueList)
                    std::cout << queueName << std::endl;
                break;
            }
        case SyncAsyncMode::Async:
            {
                auto client = dynamic_cast<ConsumerApiClientAsync*>(mApiClient.get());
                auto ql = [](QueueList aQueueList){
                    std::cout << "Queues:" << std::endl;
                    for (auto queueName : aQueueList)
                        std::cout << queueName << std::endl;
                };
                client->GetQueueList(ql);
                break;
            }
        default:
            assert(0);
        }
        break;
    case ConsumerProducerMode::Producer:
        {
            auto client = dynamic_cast<ProducerApiClient*>(mApiClient.get());
            client->StartQueueSession("TestQueue");
            client->Enqueue("str1");
            client->Enqueue("str2");
            client->Enqueue("str3");
            break;
        }
    default:
        assert(0);
    }
}
