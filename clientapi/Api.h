#pragma once

#include <functional>

#include "Handle.h"

// Sync Api, consumer
namespace QueueApiConsumerSync
{
    Handle Connect(const char* host, int port, char** errorMessage);

    /// list is separated by \0
    std::size_t GetQueueList(Handle handle, char** list);

    void StartQueueSession(Handle handle, const char* queueName, std::size_t offset = 0);

    bool Dequeue(Handle handle, char** str, std::size_t *offset);

    void Disconnect(Handle handle);
}

// Async Api, consumer
namespace QueueApiConsumerAsync
{
    void Connect(const char* host, int port, std::function<void(Handle handle, char* errorMessage)> callback);

    void GetQueueList(Handle handle, std::function<void(const char* list, std::size_t size)> callback);

    void StartQueueSession(Handle handle, std::function<void()> callback, const char* queueName, std::size_t offset = 0);

    void Dequeue(Handle handle, std::function<void(const char* str, std::size_t offset)> callback);

    void Disconnect(Handle handle);
}

// Sync Api, producer
namespace QueueApiProducerSync
{
    Handle Connect(const char* host, int port, char** errorMessage);

    void StartQueueSession(Handle handle, const char* queueName);

    void Enqueue(Handle handle, const char* str);

    void Disconnect(Handle handle);
}
