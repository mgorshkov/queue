#pragma once

#include <boost/system/system_error.hpp>

#include "IApiClient.h"

class IProducerApiClient : public IApiClient
{
public:
    virtual boost::system::error_code Connect(const ServerData& aServerData) = 0;
    virtual void StartQueueSession(const std::string& aQueueName) = 0;
    virtual void Enqueue(const DataType& aData) = 0;
    virtual void Disconnect() = 0;
};
