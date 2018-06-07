#pragma once

#include "IProducerApiClient.h"
#include "Defines.h"

class ProducerApiClient : public IProducerApiClient
{
public:
    ProducerApiClient(ba::io_service& aIoService);

    void Connect(const ServerData& aServerData);
    void StartQueueSession(const std::string& aQueueName);
    void Enqueue(const DataType& aData);
    void Disconnect();

private:
    ba::io_service& mIoService;
    ba::ip::tcp::socket mSocket;
};
