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

        auto block = PgConnectionBlock{new PgConnection, true};
        m_pool.push_back(block);

        return block.connection;
    }

    void put(PgConnection* object)
    {
        for (size_t i = 0; i < m_pool.size(); ++i)
        {
            if (m_pool[i].connection == object)
            {
                m_pool[i].busy = false;
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
