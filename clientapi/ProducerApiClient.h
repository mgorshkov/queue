#pragma once

#include "IProducerApiClient.h"
#include "Defines.h"

#include <boost/asio.hpp>

namespace ba = boost::asio;

class ProducerApiClient : public IProducerApiClient
{
public:
    ProducerApiClient();

    boost::system::error_code Connect(const ServerData& aServerData);
    void StartQueueSession(const std::string& aQueueName);
    void Enqueue(const DataType& aData);
    void Disconnect();

private:
    ba::io_service mIoService;
    ba::ip::tcp::socket mSocket;
};
