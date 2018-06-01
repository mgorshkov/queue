#include <string>
#include <memory>

#include "Defines.h"
#include "IApiClient.h"

class Client
{
public:
    Client(const ServerData& aServerData,
        ConsumerProducerMode aConsumerProducerMode,
        SyncAsyncMode aSyncAsyncMode);

    void Run();

private:
    void CreateApiClient();

private:
    ba::io_service mIoService;

    std::unique_ptr<IApiClient> mApiClient;
    ServerData mServerData;
    ConsumerProducerMode mConsumerProducerMode;
    SyncAsyncMode mSyncAsyncMode;
};

