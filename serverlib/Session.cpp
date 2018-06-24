#include <iostream>

#include "Session.h"
#include "ProtocolSerializer.h"

using ba::ip::tcp;

Session::Session(tcp::socket aSocket, CommandExecutorPtr aCommandExecutor, ba::io_service& aIoService)
    : mSocket(std::move(aSocket))
    , mContext(aCommandExecutor)
    , mIoService(aIoService)
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Session, this==" << this << std::endl;
#endif
}

Session::~Session()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::~Session, this==" << this << std::endl;
#endif
}

void Session::Start()
{
    mWriteThread = std::move(std::thread(&Session::ProcessWrite, this));
    mContext.Start();
    DoRead();
}

void Session::Stop()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Stop 1, this==" << this << std::endl;
#endif
    mSocket.close();
    mContext.Stop();
    mDone = true;
    if (mWriteThread.joinable())
        mWriteThread.join();
#ifdef DEBUG_PRINT
    std::cout << "Session::Stop 2, this==" << this << std::endl;
#endif
}

void Session::ProcessWrite()
{
    try
    {
        while (!mDone.load())
        {
            if (GetWriteQueue())
                DoWrite();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

void Session::DoRead()
{
    auto self(shared_from_this());

#ifdef DEBUG_PRINT
    std::cout << "Session::DoProcessCommand 1, this==" << this << std::endl;
#endif

    mSocket.async_read_some(ba::buffer(mBuffer),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
#ifdef DEBUG_PRINT
            std::cout << "Session::DoProcessCommand 2, this==" << this << ", ec=" << ec << ", mBuffer=" << &mBuffer << ", length=" << length << std::endl;
#endif
            if (!ec)
            {
                Deliver(length);

                DoRead();
            }
            else
                Stop();
        });
}

bool Session::GetWriteQueue()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 1, this==" << this << std::endl;
#endif
    auto statuses = mContext.GetOutboundQueue();
#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 2, this==" << this << std::endl;
#endif
    if (statuses.empty())
        return false;
#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 3, this==" << this << std::endl;
#endif
    std::stringstream str;
    for (const auto& status : statuses)
        ProtocolSerializer::Serialize(status, str);
#ifdef DEBUG_PRINT
    std::cout << "Session::GetWriteQueue 4, this==" << this << ", str=" << str.str() << std::endl;
#endif
    mWriteMsgs.push_back(str.str());
    return true;
}
  
void Session::DoWrite()
{
#ifdef DEBUG_PRINT
    std::cout << "Session::DoWrite 1, this==" << this << std::endl;
#endif
    auto self(shared_from_this());

    boost::asio::async_write(mSocket,
        boost::asio::buffer(mWriteMsgs.front().c_str(),
            mWriteMsgs.front().size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
#ifdef DEBUG_PRINT
           std::cout << "Session::DoWrite 2, this==" << this << ", ec=" << ec << ", msg=" << mWriteMsgs.front().c_str() << std::endl;
#endif
           if (ec)
               Stop();
           else
           {
#ifdef DEBUG_PRINT
               std::cout << "Session::DoWrite 3, this==" << this << ", ec=" << ec << std::endl;
#endif
           }
       });
     mWriteMsgs.pop_front();
#ifdef DEBUG_PRINT
     std::cout << "Session::DoWrite 4, this==" << this << ", ec=" << ec << std::endl;
#endif
}

void Session::Deliver(std::size_t aLength)
{
#ifdef DEBUG_PRINT
    std::cout << "Session::Deliver, this==" << this << ", mReadMsg.data()=" << mReadMsg.data() << ", mReadMsg.size()=" << mReadMsg.size() << std::endl;
#endif

    mContext.ProcessData(mBuffer, aLength);
}
