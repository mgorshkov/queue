#pragma once

#include "IProducerApiServer.h"

class ProducerApiServer
{
public:
    void Connect(const std::string& aQueueName);
    void Enqueue(const DataType& aData);

private:
    boost::asio::io_service mIoService;
    tcp::socket mSocket;
};
