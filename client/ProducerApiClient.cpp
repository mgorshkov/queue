#include "ProducerApiClient.h"

// sync client
ProducerApiClientSync::ProducerApiClientSync()
    : mProtocolSerializer(
        [&mSocket](ba::buffer& aBuffer)
        {
            mSocket.read_some(aBuffer);
        },
        [&mSocket](const ba::buffer& aBuffer)
        {
            mSocket.write(aBuffer);
        })
{
}

void ProducerApiClient::Connect(const ServerData& aServerData, const std::string& aQueueName)
{
    boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::address::from_string(aServerIp), aServerPort);
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

