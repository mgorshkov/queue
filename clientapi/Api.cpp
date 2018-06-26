#include "Api.h"
#include "ConsumerApiClient.h"
#include "ProducerApiClient.h"

static IApiClient* CreateApiClient(bool producer, bool sync)
{
    IApiClient* apiClient = 0;

    if (producer)
        apiClient = new ProducerApiClient();
    else
    {
        if (sync)
            apiClient = new ConsumerApiClientSync();
        else
            apiClient = new ConsumerApiClientAsync();
    }

    return apiClient;
}

namespace QueueApiConsumerSync
{
    Handle Connect(const char* host, int port, char** errorMessage)
    {
        auto apiClient = CreateApiClient(false, true);

        ServerData serverData{host, port};
        auto client = dynamic_cast<ConsumerApiClientSync*>(apiClient);
        assert (client);
        boost::system::error_code error = client->Connect(serverData);
        if (error)
        {
            *errorMessage = new char[error.message().length() + 1];
            strcpy(*errorMessage, error.message().c_str());
        }

        return client;
    }

    /// list is separated by \0
    std::size_t GetQueueList(Handle handle, char** list)
    {
#ifdef DEBUG_PRINT
        std::cout << "GetQueueList" << std::endl;
#endif
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert(client);

        QueueList queueList = client->GetQueueList();
#ifdef DEBUG_PRINT
        for (auto q : queueList)
            std::cout << q << std::endl;
#endif    
        std::size_t size = 0;
        for (auto queueName : queueList)
            size += queueName.length() + 1;

        *list = new char[size];
        char* ptr = *list;
        for (auto queueName : queueList)
        {
            strcpy(ptr, queueName.c_str());
            ptr += queueName.size() + 1;
        }

        return size;
    }

    void StartQueueSession(Handle handle, const char* queueName, std::size_t offset)
    {
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert (client);

        client->StartQueueSession(queueName, offset);
    }

    bool Dequeue(Handle handle, char** str, std::size_t* offset)
    {
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert(client);

        auto item = client->Dequeue();
        if (item.mOffset == static_cast<std::size_t>(-1))
            return false;
        *str = new char[item.mData.size() + 1];
        strcpy(*str, item.mData.c_str());
        *offset = item.mOffset;
        return true;
    }

    void Disconnect(Handle handle)
    {
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert (client);

        client->Disconnect();
    }
}

namespace QueueApiConsumerAsync
{
    Handle Connect(const char* host, int port, std::function<void(bool, char* errorMessage)> callback)
    {
        auto apiClient = CreateApiClient(false, false);

        auto client = dynamic_cast<ConsumerApiClientAsync*>(apiClient);
        assert(client);

        auto callbackLocal = [callback](const boost::system::error_code& error)
        {
            char* errorMessage = nullptr;
            if (error)
            {
                errorMessage = new char[error.message().size() + 1];
                strcpy(errorMessage, error.message().c_str());
            }
            callback(!error, errorMessage);
        };

        ServerData serverData{host, port};
        client->Connect(serverData, callbackLocal);
        return apiClient;
    }

    /// list is separated by \0
    void GetQueueList(Handle handle, std::function<void(const char* list, std::size_t size)> callback)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);

        auto callbackLocal = [callback](const QueueList& queueList)
        {
#ifdef DEBUG_PRINT
            for (auto q : queueList)
                std::cout << q << std::endl;
#endif    
            std::size_t size = 0;
            for (auto queueName : queueList)
                size += queueName.length() + 1;

            char* list = new char[size];
            char* ptr = list;
            for (auto queueName : queueList)
            {
                strcpy(ptr, queueName.c_str());
                ptr += queueName.size() + 1;
            }

            callback(list, size);
        };

        client->GetQueueList(callbackLocal);
    }

    void StartQueueSession(Handle handle, std::function<void()> callback, const char* queueName, std::size_t offset)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);
        client->StartQueueSession(callback, queueName, offset);
    }

    void Dequeue(Handle handle, std::function<void(const char* str, std::size_t offset)> callback)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);
        auto callbackLocal = [callback](const Item& item)
        {
            char *str = new char[item.mData.size()];
            strcpy(str, item.mData.c_str());
            std::size_t offset = item.mOffset;
            return callback(str, offset);
        };
        client->Dequeue(callbackLocal);
    }

    void Disconnect(Handle handle)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);

        client->Disconnect();
    }

    void Run(Handle handle)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);

        client->Run();
    }
}

namespace QueueApiProducerSync
{
    Handle Connect(const char* host, int port, char** errorMessage)
    {
        auto apiClient = CreateApiClient(true, true);

        ServerData serverData{host, port};
        auto client = dynamic_cast<ProducerApiClient*>(apiClient);
        assert (client);
        boost::system::error_code error = client->Connect(serverData);
        if (error)
        {
            *errorMessage = new char[error.message().length() + 1];
            strcpy(*errorMessage, error.message().c_str());
        }

        return client;
    }

    void StartQueueSession(Handle handle, const char* queueName)
    {
        auto client = dynamic_cast<ProducerApiClient*>(handle);
        assert (client);

        client->StartQueueSession(queueName);
    }

    void Enqueue(Handle handle, const char* str)
    {
        auto client = dynamic_cast<ProducerApiClient*>(handle);
        assert(client);

        client->Enqueue(str);
    }

    void Disconnect(Handle handle)
    {
        auto client = dynamic_cast<ProducerApiClient*>(handle);
        assert (client);

        client->Disconnect();
    }
}
