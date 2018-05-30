#pragma once

#include "IProducerApiClient.h"
#include "ProtocolSerializer.h"

class ProducerApiClient : public IProducerApiClient
{
public:
    void Connect(const ServerData& aServerData);
    void StartQueueSession(const std::string& aQueueName);
    void Enqueue(const DataType& aData);
    void Disconnect();

private:
    ProtocolSerializer mProtocolSerializer;
    boost::asio::io_service mIoService;
    tcp::socket mSocket;
};
