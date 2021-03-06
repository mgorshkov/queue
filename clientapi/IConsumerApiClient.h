#pragma once

#include <functional>
#include <boost/system/system_error.hpp>
#include "IApiClient.h"

class IConsumerApiClientSync : public IApiClient
{
public:
    virtual boost::system::error_code Connect(const ServerData& aServerData) = 0;
    virtual QueueList GetQueueList() = 0;
    virtual void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual Item Dequeue() = 0;
    virtual void Disconnect() = 0;
};

class IConsumerApiClientAsync : public IApiClient
{
public:
    virtual void Connect(const ServerData& aServerData, std::function<void(const boost::system::error_code& error)> aCallback) = 0;
    virtual void GetQueueList(std::function<void(const QueueList&)> aCallback) = 0;
    virtual void StartQueueSession(std::function<void()> aCallback, const std::string& aQueueName, std::size_t aOffset) = 0;
    virtual void Dequeue(std::function<void(const Item&)> aCallback) = 0;
    virtual void Disconnect() = 0;
};
