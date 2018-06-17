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

namespace QueueApiSync
{
    Handle Connect(const char* host, int port, char** errorMessage, bool producer)
    {
        auto apiClient = CreateApiClient(producer, true);

        ServerData serverData{host, port};
        auto consumerClient = dynamic_cast<ConsumerApiClientSync*>(apiClient);
        if (consumerClient)
        {
            boost::system::error_code error = consumerClient->Connect(serverData);
            if (error)
            {
                *errorMessage = new char[error.message().length()];
                strcpy(*errorMessage, error.message().c_str());
            }

            return consumerClient;
        }
        auto producerClient = dynamic_cast<ProducerApiClient*>(apiClient);
        assert (producerClient);
        boost::system::error_code error = producerClient->Connect(serverData);
        if (error)
        {
            *errorMessage = new char[error.message().length()];
            strcpy(*errorMessage, error.message().c_str());
        }

        return producerClient;
    }

    /// consumer only, list is separated by \0
    std::size_t GetQueueList(Handle handle, char** list)
    {
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert(client);

        QueueList queueList = client->GetQueueList();

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

    /// consumer/producer, offset ignored for producer
    void StartQueueSession(Handle handle, const char* queueName, std::size_t offset)
    {
        auto clientConsumer = dynamic_cast<ConsumerApiClientSync*>(handle);
        if (clientConsumer)
        {
            clientConsumer->StartQueueSession(queueName, offset);
            return;
        }
        auto clientProducer = dynamic_cast<ProducerApiClient*>(handle);
        if (clientProducer)
        {
            clientProducer->StartQueueSession(queueName);
            return;
        }
    }

    /// for producer
    void Enqueue(Handle handle, const char* str)
    {
        auto client = dynamic_cast<ProducerApiClient*>(handle);
        assert(client);

        client->Enqueue(str);
    }

    /// for consumer
    void Dequeue(Handle handle, char** str, std::size_t** offset)
    {
        auto client = dynamic_cast<ConsumerApiClientSync*>(handle);
        assert(client);

        auto item = client->Dequeue();
        *str = new char[item.mData.size()];
        strcpy(*str, item.mData.c_str());
        *offset = new std::size_t;
        **offset = item.mOffset;
    }

    void Disconnect(Handle handle)
    {
        auto clientConsumer = dynamic_cast<ConsumerApiClientSync*>(handle);
        if (clientConsumer)
        {
            clientConsumer->Disconnect();
            return;
        }
        auto clientProducer = dynamic_cast<ProducerApiClient*>(handle);
        if (clientProducer)
        {
            clientProducer->Disconnect();
            return;
        }
    }
}
       
namespace QueueApiAsync
{
    void Connect(const char* host, int port, std::function<void(Handle handle, char* errorMessage)> callback)
    {
        auto apiClient = CreateApiClient(false, false);

        auto client = dynamic_cast<ConsumerApiClientAsync*>(apiClient);
        assert(client);

        auto callbackLocal = [callback, apiClient](const boost::system::error_code& error)
        {
            char* errorMessage = nullptr;
            if (error)
            {
                errorMessage = new char[error.message().size() + 1];
                strcpy(errorMessage, error.message().c_str());
            }
            callback(apiClient, errorMessage);
        };

        ServerData serverData{host, port};
        client->Connect(serverData, callbackLocal);
    }

    void GetQueueList(Handle handle, std::function<void(const char* list, std::size_t size)> callback)
    {
    }

    void StartQueueSession(Handle handle, std::function<void()> callback, const char* queueName, std::size_t offset)
    {
    }

    void Dequeue(Handle handle, std::function<void(const char* str, std::size_t offset)> callback)
    {
    }

    void Disconnect(Handle handle)
    {
        auto client = dynamic_cast<ConsumerApiClientAsync*>(handle);
        assert(client);

        client->Disconnect();
    }
}
