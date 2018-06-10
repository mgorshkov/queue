#include <string>
#include <memory>

#include "Defines.h"
#include "Api.h"

class Client
{
public:
    Client(const ServerData& aServerData,
        ConsumerProducerMode aConsumerProducerMode,
        SyncAsyncMode aSyncAsyncMode);

    void Run();

private:
    void RunConsumerSync();
    void RunConsumerAsync();
    void RunProducerSync();

    ServerData mServerData;
    ConsumerProducerMode mConsumerProducerMode;
    SyncAsyncMode mSyncAsyncMode;
};

