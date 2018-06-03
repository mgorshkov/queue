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
    ba::ip::tcp::socket mSocket;
    std::string mQueueName;
    std::size_t mOffset;
};

class ConsumerApiClientAsync : public IConsumerApiClientAsync
{
public:
    ConsumerApiClientAsync(ba::io_service& aIoService);

    void Connect(const ServerData& aServerData);
    void GetQueueList(std::function<QueueList(void)> aCallback);
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset);
    void Dequeue(std::function<Item (void)> aCallback);
    void Disconnect();

private:
    void DoWrite();
    void DoRead();

    ba::ip::tcp::socket mSocket;
    ba::io_service& mIoService;

    std::string mQueueName;
    std::size_t mOffset;
};

