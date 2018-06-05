#pragma once

#include <functional>
#include "IApiClient.h"
#include "Defines.h"

class IConsumerApiClientSync : public IApiClient
{
public:
    virtual QueueList GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual Item Dequeue() = 0;
};

class IConsumerApiClientAsync : public IApiClient
{
public:
    virtual void GetQueueList(std::function<void(QueueList)> aCallback) = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual void Dequeue(std::function<void(Item)> aCallback) = 0;
};
