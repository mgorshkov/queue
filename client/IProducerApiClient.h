#pragma once

class IProducerApiClient
{
public:
    virtual void Connect(const ServerData& aServerData, const std::string& aQueueName)
    virtual void Enqueue(const DataType& aData);
    virtual void Disconnect();
};
