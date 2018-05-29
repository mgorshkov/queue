#include <iostream>

#include "Client.h"

Client::Client(const ServerData& aServerData, ConsumerProducerMode aConsumerProducerMode, SyncAsyncMode aSyncAsyncMode))
    : mServerData(aServerData)
    , mConsumerProducerMode(aConsumerProducerMode)
    , mSyncAsyncMode(aSyncAsyncMode)
{
}

void CreateApiClient()
{
    switch (mConsumerProducerMode)
    {
    case ConsumerProducerMode::ModeConsumer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::ModeSync:
            mApi = make_unique<ConsumerSyncApiClient>();
            break;
        case SyncAsyncMode::ModeAsync:
            mApi = make_unique<ConsumerAsyncApiClient>();
            break;
        }
        break;
    case ConsumerProducerMode::ModeProducer:
        switch (mSyncAsyncMode)
        {
        case SyncAsyncMode::ModeSync:
            mApi = make_unique<ProducerSyncApiClient>();
            break;
        case SyncAsyncMode::ModeAsync:
            mApi = make_unique<ProducerAsyncApiClient>();
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
