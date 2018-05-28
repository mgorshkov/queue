#include "ConsumerApiClient.h"

// sync client
void ConsumerApiClientSync::Connect(const ServerData& aServerData)
{
    boost::asio::ip::tcp::endpoint endPoint(
        boost::asio::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

std::vector<std::string> ConsumerApiClientSync::GetQueueList()
{
    mSocket.write(ProtocolSerializer::Serialize(ProtocolSerializer::Message::QueueList));
    return ProtocolSerializer::Deserialize(mSocket, ProtocolSerializer::Message::QueueList);
}

Item ConsumerApiClientSync::Dequeue()
{
    mSocket.write(ProtocolSerializer::Serialize(ProtocolSerializer::Message::Dequeue));
    return ProtocolSerializer::Deserialize(mSocket, ProtocolSerializer::Message::Dequeue);
}

void ComsumerApiClientSync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    mSocket.write(ProtocolSerializer::Serialize(Protocol::Message::StartQueueSession, aQueueName, aOffset));
}

void ConsumerApiClientSync::Disconnect()
{
    mSocket.close();
}

// async client
void ConsumerApiClientAsync::Connect(const ServerData& aServerData)
{
    boost::asio::ip::tcp::endpoint endPoint(
        boost::asio::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

void ConsumerApiClientAsync::GetQueueList(std::function<void (std::vector<std::string>)> aCallback)
{
    mIoService.post(
        [this, msg]()
        {
            bool writeInProgress = !mWriteMsgs.empty();
            auto msg = ProtocolSerializer::Serialize(ProtocolSerializer::Message::QueueList);
            mWriteMsgs.push_back(msg);
            if (!writeInProgress)
            {
                DoWrite();
                DoRead();
            }
        });
}

void ConsumerApiClientAsync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
   mIoService.post(
        [this, msg]()
        {
            bool writeInProgress = !mWriteMsgs.empty();
            auto msg = ProtocolSerializer::Serialize(ProtocolSerializer::Message::StartQueueSession, aQueueName, aOffset);
            mWriteMsgs.push_back(msg);
            if (!writeInProgress)
            {
                DoWrite();
                DoRead();
            }
        });
}

void ConsumerApiClientAsync::Dequeue(std::function<void (const Item&)> aCallback)
{
   mIoService.post(
        [this, msg]()
        {
            bool writeInProgress = !mWriteMsgs.empty();
            auto msg = ProtocolSerializer::Serialize(ProtocolSerializer::Message::Dequeue);
            mWriteMsgs.push_back(msg);
            if (!writeInProgress)
            {
                DoWrite();
                DoRead();
            }
        });
}

void ConsumerApiClientAsync::Disconnect()
{
    mIoService.post([this]()
        {
            mSocket.close();
        });
}

void ConsumerApiClientAsync::DoWrite()
{
}

void ConsumerApiClientAsync::DoRead()
{
}
