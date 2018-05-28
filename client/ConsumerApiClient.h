#pragma once

#include <boost/asio.hpp>

#include "IConsumerApiClient.h"

namespace ba = boost::asio;

class ConsumerApiClientSync : public IConsumerApiClientSync
{
public:
    void Connect(const ServerData& aServerData);
    std::vector<std::string> GetQueueList();
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset);
    Item Dequeue();
    void Disconnect();

private:
    ba::io_service mIoService;
    tcp::socket mSocket;
};

class ConsumerApiClientAsync : public IConsumerApiClientAsync
{
public:
    void Connect(const ServerData& aServerData);
    void GetQueueList(std::function<std::vector<std::string>()> aCallback) = 0;
    void StartQueueSession(const std::string& aQueueName, std::size_t aOffset) = 0;
    void Dequeue(std::function<Item()> aCallback) = 0;
    void Disconnect() = 0;

private:
    ba::io_service mIoService;
    tcp::socket mSocket;
};

