class ProducerApi
{
public:
    void Connect(const std::string& aServerIp, int aServerPort, const std::string& aQueueName);
    void Enqueue(const Item& aItem);
    void Disconnect();

private:
    boost::asio::io_service mIoService;
    tcp::socket mSocket;
};

class ConsumerApi
{
public:
    void Connect(const std::string& aServerIp, int aServerPort, const std::string& aQueueName);
    Item Dequeue();
    void Disconnect();

private:
    boost::asio::io_service mIoService;
    tcp::socket mSocket;
};

