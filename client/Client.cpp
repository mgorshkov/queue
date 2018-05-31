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
            mApiClient = std::make_unique<ConsumerApiClientSync>();
            break;
        case SyncAsyncMode::Async:
            mApiClient = std::make_unique<ConsumerApiClientAsync>();
            break;
        }
        break;
    case ConsumerProducerMode::Producer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::Sync:
            mApiClient = std::make_unique<ProducerApiClientSync>();
            break;
        case SyncAsyncMode::Async:
            mApiClient = std::make_unique<ProducerApiClientAsync>();
            break;
        }
        break;
    }
}

void Client::Run()
{
    CreateApiClient();
    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::ModeConsumer:
        break;
    case ConsumerProducerMode::ModeProducer:
        break;
    }
}
