#include "ProducerApiServer.h"

void ProducerApiClient::Connect(const ServerData& aServerData, const std::string& aQueueName)
{
    boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

void ProducerApiClient::Enqueue(const Item& aItem)
{
    boost::asio::write(mSocket, boost::asio::buffer(aMsg.c_str(), aMsg.length()));
}

void ProducerApiClient::Disconnect()
{
    mSocket.close();
}

