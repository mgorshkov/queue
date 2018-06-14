#pragma once

#include "IApiClient.h"

class IProducerApiClient : public IApiClient
{
public:
    virtual void Connect(const ServerData& aServerData) = 0;
    virtual void StartQueueSession(const std::string& aQueueName) = 0;
    virtual void Enqueue(const DataType& aData) = 0;
};