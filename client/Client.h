#include <string>

#include "Defines.h"

class Client
{
public:
    Client(ServerData aServerData, ConsumerProducerMode aConsumerProducerMode, SyncAsyncMode aSyncAsyncMode);

    void Run();

private:
    void CreateApiClient();

private:
    std::unique_ptr<IApiClient> mApiClient;
    ServerData mServerData;
    ConsumerProducerMode mConsumerProducerMode;
    SyncAsyncMode mSyncAsyncMode;
};

