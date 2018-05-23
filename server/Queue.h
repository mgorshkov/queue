#include <mutex>

#include "Defines.h"

class Queue
{
public:
    void Enqueue(const Item& mItem);
    ItemPtr Dequeue();

private:
    std::mutex mQueueMutex;
    ItemQueue mQueue;
};

