#pragma once

#include <functional>
#include "IApiClient.h"
#include "Defines.h"

class IConsumerApiClientSync : public IApiClient
{
public:
    virtual QueueList GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual ItemPtr Dequeue() = 0;
};

class IConsumerApiClientAsync : public IApiClient
{
public:
    virtual void GetQueueList(std::function<QueueList()> aCallback) = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual void Dequeue(std::function<ItemPtr()> aCallback) = 0;
};
