#include "ClientApi.h"

void ProducerApi::Connect(const std::string& aServerIp, int aServerPort, const std::string& aQueueName)
{
    boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::address::from_string(aServerIp), aServerPort);
    mSocket.connect(endPoint);
}

void ProducerApi::Enqueue(const Item& aItem)
{
    boost::asio::write(mSocket, boost::asio::buffer(aMsg.c_str(), aMsg.length()));
}

void ProducerApi::Disconnect()
{
    mSocket.close();
}

void ConsumerApi::Connect(const std::string& aServerIp, const std::string& aQueueName)
{
    boost::asio::ip::tcp::endpoint endPoint(boost::asio::ip::address::from_string(aServerIp), aServerPort);
    mSocket.connect(endPoint);
}

Item ConsumerApi::Dequeue()
{
}

void ConsumerApi::Disconnect()
{
    mSocket.close();
}


