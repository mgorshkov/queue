#pragma once

#include "IApiClient.h"

class IProducerApiClient : public IApiClient
{
public:
    virtual void StartQueueSession(const std::string& aQueueName) = 0;
    virtual void Enqueue(const DataType& aData) = 0;
};
