#include "ConsumerApiClient.h"
#include "ProtocolSerializer.h"

// sync client
ConsumerApiClientSync::ConsumerApiClientSync(ba::io_service& aIoService)
    : mIoService(aIoService)
    , mSocket(aIoService)
{
}

void ConsumerApiClientSync::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mServerIp, aServerData.mServerPort);
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);
    ba::ip::tcp::endpoint endPoint(*it);
    mSocket.connect(endPoint);
}

QueueList ConsumerApiClientSync::GetQueueList()
{
    auto message = std::make_shared<QueueListMessage>();
    {
        ba::streambuf buffer;
        std::ostream stream(&buffer);
        ProtocolSerializer::Serialize(message, stream);
        ba::write(mSocket, buffer);
    }
    {
        constexpr const int MAXSIZE = 256;
        std::array<char, MAXSIZE> buffer;
        size_t len = mSocket.read_some(ba::buffer(buffer));
        std::istringstream stream(std::string(&buffer[0], len));
        auto response = ProtocolSerializer::Deserialize(stream);
        auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(response);
        return queueListMessage->mQueueList;
    }
}

void ConsumerApiClientSync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    auto message = std::make_shared<StartQueueSessionMessage>(aQueueName, aOffset);
    ba::streambuf buffer;
    std::ostream stream(&buffer);
    ProtocolSerializer::Serialize(message, stream);
    ba::write(mSocket, buffer);
}

Item ConsumerApiClientSync::Dequeue()
{
    auto message = std::make_shared<DequeueMessage>();
    {
        ba::streambuf buffer;
        std::ostream stream(&buffer);
        ProtocolSerializer::Serialize(message, stream);
        ba::write(mSocket, buffer);
    }
    {
        constexpr const int MAXSIZE = 256;
        std::array<char, MAXSIZE> buffer;
        size_t len = mSocket.read_some(ba::buffer(buffer));
        std::istringstream stream(std::string(&buffer[0], len));
        auto response = ProtocolSerializer::Deserialize(stream);
        auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(response);
        return dequeueMessage->mItem;
    }
}

void ConsumerApiClientSync::Disconnect()
{
    mSocket.close();
}

// async client
ConsumerApiClientAsync::ConsumerApiClientAsync(ba::io_service& aIoService)
    : mIoService(aIoService)
    , mSocket(aIoService)
{
}

void ConsumerApiClientAsync::Connect(const ServerData& aServerData, std::function<void(const boost::system::error_code& error)> aCallback)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mServerIp, aServerData.mServerPort);
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);
    ba::ip::tcp::endpoint endPoint(*it);
    mSocket.async_connect(endPoint, aCallback);
}

void ConsumerApiClientAsync::GetQueueList(std::function<void(QueueList)> aCallback)
{
/*    mIoService.post(
        [this, msg]()
        {
            auto message = std::make_shared<QueueListMessage>();
            ba::streambuf buffer;
            {
                std::ostream stream(&buffer);
                ProtocolSerializer::Serialize(message, stream);
                ba::write(mSocket, buffer);
            }

            bool writeInProgress = !mWriteMsgs.empty();
            auto msg = ProtocolSerializer::Serialize(ProtocolSerializer::Message::QueueList);
            mWriteMsgs.push_back(msg);
            if (!writeInProgress)
            {
                DoWrite();
                DoRead();
            }
        });*/
}

void ConsumerApiClientAsync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    mQueueName = aQueueName;
    mOffset = aOffset;
}

void ConsumerApiClientAsync::Dequeue(std::function<void(Item)> aCallback)
{/*
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
        });*/
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
