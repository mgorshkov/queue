#pragma once

#include <functional>

class IConsumerApiClientSync
{
public:
    virtual void Connect(const ServerData& aServerData) = 0;
    virtual QueueList GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual Item Dequeue() = 0;
    virtual void Disconnect() = 0;
};

class IConsumerApiClientAsync
{
public:
    virtual void Connect(const ServerData& aServerData) = 0;
    virtual void GetQueueList(std::function<QueueList()> aCallback) = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual void Dequeue(std::function<Item()> aCallback) = 0;
    virtual void Disconnect() = 0;
};
