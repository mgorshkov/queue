#pragma once

#include "Defines.h"

#include "IConsumerApiClient.h"

class ConsumerApiClientSync : public IConsumerApiClientSync
{
public:
    ConsumerApiClientSync(ba::io_service& aIoService);

    void Connect(const ServerData& aServerData);
    QueueList GetQueueList();
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset);
    Item Dequeue();
    void Disconnect();

private:
    ba::io_service& mIoService;
    ba::ip::tcp::socket mSocket;
    std::string mQueueName;
    std::size_t mOffset;
};

class ConsumerApiClientAsync : public IConsumerApiClientAsync
{
public:
    ConsumerApiClientAsync(ba::io_service& aIoService);

    void Connect(const ServerData& aServerData, std::function<void(const boost::system::error_code& error)> aCallback);
    void GetQueueList(std::function<void(QueueList)> aCallback);
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset);
    void Dequeue(std::function<void(Item)> aCallback);
    void Disconnect();

private:
    void DoWrite();
    void DoRead();

    ba::io_service& mIoService;
    ba::ip::tcp::socket mSocket;

    std::string mQueueName;
    std::size_t mOffset;
};

