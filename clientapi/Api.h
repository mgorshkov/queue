#pragma once

#include <functional>

#include "Handle.h"

// Sync Api, consumer/producer
namespace QueueApiSync
{
    Handle Connect(const char* host, int port, char** errorMessage, bool producer = true);

    /// consumer only, list is separated by \0
    std::size_t GetQueueList(Handle handle, char** list);

    /// consumer/producer, offset ignored for producer*
    void StartQueueSession(Handle handle, const char* queueName, std::size_t offset = static_cast<std::size_t>(-1));
    /// for producer
    void Enqueue(Handle handle, const char* str);
    /// for consumer
    void Dequeue(Handle handle, char** str, std::size_t** offset);

    void Disconnect(Handle handle);
}

// Async Api, consumer
namespace QueueApiAsync
{
    void Connect(const char* host, int port, std::function<void(Handle handle, char* errorMessage)> callback);

    void GetQueueList(Handle handle, std::function<void(const char* list, std::size_t size)> callback);

    void StartQueueSession(Handle handle, std::function<void()> callback, const char* queueName, std::size_t offset = static_cast<std::size_t>(-1));

    void Dequeue(Handle handle, std::function<void(const char* str, std::size_t offset)> callback);

    void Disconnect(Handle handle);
}
