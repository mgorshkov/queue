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
    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::Consumer:
        break;
    case ConsumerProducerMode::Producer:
        break;
    default:
        assert(0);
    }
}
