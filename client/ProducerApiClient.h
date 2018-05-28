#pragma once

#include "IProducerApiClient.h"

class ProducerApiClient : public IProducerApiClient
{
public:
    void Connect(const ServerData& aServerData, const std::string& aQueueName);
    void Enqueue(const DataType& aData);
    void Disconnect();

private:
    boost::asio::io_service mIoService;
    tcp::socket mSocket;
};
