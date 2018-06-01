#include "ConsumerApiClient.h"
#include "ProtocolSerializer.h"

// sync client
ConsumerApiClientSync::ConsumerApiClientSync(ba::io_service& aIoService)
    : mSocket(aIoService)
{
}

void ConsumerApiClientSync::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::endpoint endPoint(
        ba::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

QueueList ConsumerApiClientSync::GetQueueList()
{
    QueueListMessage message{};
    ba::streambuf buffer;
    {
        std::ostream stream(&buffer);
        ProtocolSerializer::Serialize(message, stream);
        ba::write(mSocket, buffer);
    }
    {
        size_t len = mSocket.read_some(buffer);
        std::istream stream(&buffer);
        auto response = ProtocolSerializer::Deserialize(stream);
        auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(response);
        return queueListMessage->mQueueList;
    }
}

ItemPtr ConsumerApiClientSync::Dequeue()
{
    mProtocolSerializer.Serialize(ProtocolSerializer::Message::Dequeue);
    return mProtocolSerializer.Deserialize(ProtocolSerializer::Message::Dequeue);
}

void ConsumerApiClientSync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    mProtocolSerializer.Serialize(Protocol::Message::StartQueueSession, aQueueName, aOffset);
}

void ConsumerApiClientSync::Disconnect()
{
    mSocket.close();
}

// async client
ConsumerApiClientAsync::ConsumerApiClientSync(ba::io_service& aIoService)
    : mSocket(aIoService)
{
}

void ConsumerApiClientAsync::Connect(const ServerData& aServerData)
{
    boost::asio::ip::tcp::endpoint endPoint(
        boost::asio::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

void ConsumerApiClientAsync::GetQueueList(std::function<QueueList(void)> aCallback)
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
            StartQueueSessionMessage message{aQueueName, aOffset};
            auto msg = ProtocolSerializer::Serialize(message);
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
