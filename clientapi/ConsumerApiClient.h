#pragma once

#include <boost/asio.hpp>

#include "Defines.h"
#include "IConsumerApiClient.h"

namespace ba = boost::asio;

class ConsumerApiClientSync : public IConsumerApiClientSync
{
public:
    ConsumerApiClientSync();

    boost::system::error_code Connect(const ServerData& aServerData);
    QueueList GetQueueList();
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset);
    Item Dequeue();
    void Disconnect();

private:
    ba::io_service mIoService;
    ba::ip::tcp::socket mSocket;
};

class ConsumerApiClientAsync : public IConsumerApiClientAsync
{
public:
    ConsumerApiClientAsync();

    void Connect(const ServerData& aServerData, std::function<void(const boost::system::error_code& error)> aCallback);
    void GetQueueList(std::function<void(const QueueList&)> aCallback);
    void StartQueueSession(std::function<void()> aCallback, const std::string& aQueueName, std::size_t aOffset);
    void Dequeue(std::function<void(const Item&)> aCallback);
    void Disconnect();

    void Run();

private:
    std::function<void(const boost::system::error_code& error)> mConnectCallback;
    ba::ip::tcp::resolver::iterator mResolverIterator;
    void ConnectInternal();

    ba::io_service mIoService;
    ba::ip::tcp::socket mSocket;
    BufferType mReadBuffer;
    BufferType mWriteBuffer;
};

