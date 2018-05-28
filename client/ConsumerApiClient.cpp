#include "ConsumerApiClient.h"

// async client
void ConsumerApiClientSync::Connect(const ServerData& aServerData)
{
    boost::asio::ip::tcp::endpoint endPoint(
        boost::asio::ip::address::from_string(aServerData.mServerIp), aServerData.mServerPort);
    mSocket.connect(endPoint);
}

std::vector<std::string> ConsumerApiClientSync::GetQueueList()
{
    ba::write(sock, ba::buffer("ping", 4));
    char data[4];
    size_t len = sock.read_some(ba::buffer(data));
    std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
}

void ComsumerApiClientSync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
    ba::write(sock, ba::buffer("ping", 4));
}

Item ConsumerApiClientSync::Dequeue()
{
    ba::write(sock, ba::buffer("ping", 4));
    char data[4];
    size_t len = sock.read_some(ba::buffer(data));
    std::cout << "receive " << len << "=" << std::string{data, len} << std::endl;
}

void ConsumerApiClientSync::Disconnect()
{
    mSocket.close();
}

// sync client
void ConsumerApiClientAsync::Connect(const ServerData& aServerData)
{
}

void ConsumerApiClientAsync::GetQueueList(std::function<std::vector<std::string>()> aCallback)
{
    mIoService.post(
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
}

void ConsumerApiClientAsync::StartQueueSession(const std::string& aQueueName, std::size_t aOffset)
{
}

void ConsumerApiClientAsync::Dequeue(std::function<Item()> aCallback)
{
}

void ConsumerApiClientAsync::Disconnect()
{
    io_service_.post([this]() { socket_.close(); });
}
