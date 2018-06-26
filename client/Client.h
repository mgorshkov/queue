#include <string>
#include <memory>

#include "Defines.h"
#include "Api.h"

class Client
{
public:
    Client(const ServerData& aServerData,
        ConsumerProducerMode aConsumerProducerMode,
        SyncAsyncMode aSyncAsyncMode,
        int aNumber,
        std::size_t aStartOffset);

    void Run();

private:
    void RunConsumerSync();
    void RunConsumerAsync();
    void RunProducerSync();

    ServerData mServerData;
    ConsumerProducerMode mConsumerProducerMode;
    SyncAsyncMode mSyncAsyncMode;
    int mNumber;
    std::size_t mStartOffset;
};

