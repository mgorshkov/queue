#pragma once

#include <functional>

class IConsumerApiClientSync
{
public:
    virtual void Connect(const ServerData& aServerData) = 0;
    virtual std::vector<std::string> GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual Item Dequeue() = 0;
    virtual void Disconnect() = 0;
};

class IConsumerApiClientAsync
{
public:
    virtual void Connect(const ServerData& aServerData) = 0;
    virtual void GetQueueList(std::function<std::vector<std::string>()> aCallback) = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual void Dequeue(std::function<Item()> aCallback) = 0;
    virtual void Disconnect() = 0;
};
