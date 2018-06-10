#include "ProducerApiClient.h"

#include "Message.h"
#include "ProtocolSerializer.h"

ProducerApiClient::ProducerApiClient()
    : mSocket(mIoService)
{
}

void ProducerApiClient::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mHost, std::to_string(aServerData.mPort));
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);

    boost::system::error_code error = boost::asio::error::host_not_found;
    ba::ip::tcp::resolver::iterator end;

    while (error && it != end)
    {
        mSocket.close();
        ba::ip::tcp::endpoint endPoint(*it++);
        mSocket.connect(endPoint, error);
    }
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

