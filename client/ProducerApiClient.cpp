#include "ProducerApiClient.h"

#include "Message.h"
#include "ProtocolSerializer.h"

ProducerApiClient::ProducerApiClient(ba::io_service& aIoService)
    : mSocket(aIoService)
{
}

void ProducerApiClient::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::endpoint endPoint(ba::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

void ProducerApiClient::StartQueueSession(const std::string& aQueueName)
{
    mQueueName = aQueueName;
}

void ProducerApiClient::Enqueue(const DataType& aData)
{
    auto message = std::make_shared<EnqueueMessage>(aData);
    ba::streambuf buffer;
    std::ostream stream(&buffer);
    ProtocolSerializer::Serialize(message, stream);
    ba::write(mSocket, buffer);
}

void ProducerApiClient::Disconnect()
{
    mSocket.close();
}

