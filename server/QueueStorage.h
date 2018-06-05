#pragma once

#include <thread>
#include <queue>
#include <fstream>

class QueueStorage
{
public:
    QueueStorage();
    
    void Load(const std::string& aStorageFileName);

    void AddItem(const Item& aItem);

private:
    std::queue<Item> mQueue;

    std::fstream mStorage;
    std::thread mThread;
};

