class ThreadPool
{
public:
    std::unique_ptr<std::thread> get()
    {
        for (auto& poolElem : mPool)
        {
            if (!poolElem.busy)
            {
                poolElem.busy = true;
                return poolElem.mThread;
            }
        }

        auto block = ThreadInfo{std::make_unique(std::thread), true};
        mPool.push_back(block);

        return block.connection;
    }

    void put(ThreadInfo* object)
    {
        for (size_t i = 0; i < mPool.size(); ++i)
        {
            if (mPool[i].connection == object)
            {
                mPool[i].busy = false;
                break;
            }
        }
    }

    ~PgConnectionPool()
    {
        for (const auto &i : m_pool)
        {
            std::cout << i.connection << std::endl;
            delete i.connection;
        }
    }

private:
    struct ThreadInfo
    {
        std::unique_ptr<std::thread> mThread;
        bool mBusy;
    };

    std::vector<ThreadInfo> mPool;
};


int main(int, char const **)
{
    PgConnectionPool pool;

    auto report_conn = pool.get();
    pool.put(report_conn);

    auto admin_conn = pool.get();
    pool.put(admin_conn);

    return 0;
}
