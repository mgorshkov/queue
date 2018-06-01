#include "ProducerApiClient.h"

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
    ProtocolSerializer.Serialize(Protocol::Message::StartQueueSession, aQueueName, aOffset);
}

void ProducerApiClient::Enqueue(const Item& aItem)
{
    boost::asio::write(mSocket, boost::asio::buffer(aMsg.c_str(), aMsg.length()));
}

void ProducerApiClient::Disconnect()
{
    mSocket.close();
}

