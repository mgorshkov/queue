#pragma once

#include "Defines.h"

#include "IConsumerApiClient.h"

class ConsumerApiClientSync : public IConsumerApiClientSync
{
public:
    ConsumerApiClientSync();

    void Connect(const ServerData& aServerData);
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
    void Connect(const ServerData& aServerData);
    void GetQueueList(std::function<QueueList()> aCallback) = 0;
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    void Dequeue(std::function<Item()> aCallback) = 0;
    void Disconnect() = 0;

private:
    ba::io_service mIoService;
    ba::ip::tcp::socket mSocket;
};

