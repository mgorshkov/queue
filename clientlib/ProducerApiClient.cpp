#include <iostream>

#include "ProducerApiClient.h"

#include "Message.h"
#include "ProtocolSerializer.h"

ProducerApiClient::ProducerApiClient(ba::io_service& aIoService)
    : mIoService(aIoService)
    , mSocket(aIoService)
{
}

void ProducerApiClient::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mServerIp, aServerData.mServerPort);
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);
    ba::ip::tcp::endpoint endPoint(*it);

    std::cout << "Connecting to " << endPoint.address().to_string() << ":" << endPoint.port() << "..." << std::endl;
    mSocket.connect(endPoint);
}

void ProducerApiClient::StartQueueSession(const std::string& aQueueName)
{
    auto message = std::make_shared<StartQueueSessionMessage>(aQueueName);
    ba::streambuf buffer;
    std::ostream stream(&buffer);
    ProtocolSerializer::Serialize(message, stream);
    ba::write(mSocket, buffer);
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

