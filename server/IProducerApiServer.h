#pragma once

class IProducerApiServer
{
public:
    virtual ~IProducerApiServer() = default;

    virtual void Connect(const std::string& aQueueName) = 0;
    virtual void Enqueue(const DataType& aData) = 0;
};
