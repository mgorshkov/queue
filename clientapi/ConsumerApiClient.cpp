#include <iostream>

#include "ConsumerApiClient.h"
#include "ProtocolSerializer.h"

// sync client
ConsumerApiClientSync::ConsumerApiClientSync()
    : mSocket(mIoService)
{
}

boost::system::error_code ConsumerApiClientSync::Connect(const ServerData& aServerData)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mHost, std::to_string(aServerData.mPort));
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);

    boost::system::error_code error = ba::error::host_not_found;
    ba::ip::tcp::resolver::iterator end;

    while (error && it != end)
    {
        mSocket.close();
        ba::ip::tcp::endpoint endPoint(*it++);
        std::cout << "Connecting to " << endPoint.address().to_string() << ":" << endPoint.port() << "..." << std::endl;
        mSocket.connect(endPoint, error);
    }
    return error;
}

QueueList ConsumerApiClientSync::GetQueueList()
{
    {
        ba::streambuf buffer;
        std::ostream stream(&buffer);
        auto message = std::make_shared<QueueListMessage>();
        ProtocolSerializer::Serialize(message, stream);
        ba::write(mSocket, buffer);
    }
    {
        BufferType buffer;
        size_t length = mSocket.read_some(ba::buffer(buffer));
        std::cout << "length=" << length << std::endl;
        std::stringstream stream;
        for (std::size_t i = 0; i < length; ++i)
        {
            std::cout << "buffer[i]=" << buffer[i] << std::endl;
            stream << buffer[i];
        }
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
        BufferType buffer;
        size_t length = mSocket.read_some(ba::buffer(buffer));
        std::istringstream stream(std::string(&buffer[0], length));
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
ConsumerApiClientAsync::ConsumerApiClientAsync()
    : mSocket(mIoService)
{
}

void ConsumerApiClientAsync::Connect(const ServerData& aServerData, std::function<void(const boost::system::error_code& error)> aCallback)
{
    ba::ip::tcp::resolver resolver(mIoService);
    ba::ip::tcp::resolver::query query(aServerData.mHost, std::to_string(aServerData.mPort));
    ba::ip::tcp::resolver::iterator it = resolver.resolve(query);

    boost::system::error_code error = ba::error::host_not_found;
    ba::ip::tcp::resolver::iterator end;

    while (error && it != end)
    {
        mSocket.close();
        ba::ip::tcp::endpoint endPoint(*it++);
        std::cout << "Connecting to " << endPoint.address().to_string() << ":" << endPoint.port() << "..." << std::endl;
        mSocket.async_connect(endPoint, aCallback);
        if (error)
            std::cout << "Error: " << error.message() << std::endl;
        else
            std::cout << "Success." << std::endl;
    }
}

void ConsumerApiClientAsync::GetQueueList(std::function<void(QueueList)> aCallback)
{
    mIoService.post(
        [this, aCallback]()
        {
            auto message = std::make_shared<QueueListMessage>();
            std::ostringstream stream;
            ProtocolSerializer::Serialize(message, stream);
            mSocket.async_write_some(ba::buffer(stream.str()),
                [this, aCallback](const boost::system::error_code& ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        BufferType buffer;
                        mSocket.async_read_some(ba::buffer(buffer),
                            [this, &buffer, aCallback](const boost::system::error_code& ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    std::stringstream stream;
                                    for (int i = 0; i < length; ++i)
                                        stream << buffer[i];
                                    auto response = ProtocolSerializer::Deserialize(stream);
                                    auto queueListMessage = std::dynamic_pointer_cast<QueueListMessage>(response);
                                    aCallback(queueListMessage->mQueueList);
                                }
                                else
                                   mSocket.close();
                            });
                    }
                    else
                        mSocket.close();
                });
      });
}

void ConsumerApiClientAsync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    mIoService.post(
        [this, &aQueueName, aOffset]()
        {
            auto message = std::make_shared<StartQueueSessionMessage>(aQueueName, aOffset);
            std::ostringstream stream;
            ProtocolSerializer::Serialize(message, stream);
            mSocket.async_write_some(ba::buffer(stream.str()),
                [this](const boost::system::error_code& ec, std::size_t /*length*/)
                {
                    if (ec)
                        mSocket.close();
                });
       });
}

void ConsumerApiClientAsync::Dequeue(std::function<void(Item)> aCallback)
{
    mIoService.post(
        [this, aCallback]()
        {
            auto message = std::make_shared<DequeueMessage>();
            std::ostringstream stream;
            ProtocolSerializer::Serialize(message, stream);
            mSocket.async_write_some(ba::buffer(stream.str()),
                [this, aCallback](const boost::system::error_code& ec, std::size_t length)
                {
                    if (!ec)
                    {
                        BufferType buffer;
                        mSocket.async_read_some(ba::buffer(buffer),
                            [this, &buffer, aCallback](const boost::system::error_code& ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    std::stringstream stream;
                                    for (int i = 0; i < length; ++i)
                                        stream << buffer[i];
                                    auto response = ProtocolSerializer::Deserialize(stream);
                                    auto dequeueMessage = std::dynamic_pointer_cast<DequeueMessage>(response);
                                    aCallback(dequeueMessage->mItem);
                                }
                                else
                                   mSocket.close();
                            });
                    }
                    else
                        mSocket.close();
                });
      });
}

void ConsumerApiClientAsync::Disconnect()
{
    mIoService.post([this]()
        {
            mSocket.close();
        });
}
