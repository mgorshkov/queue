#include <mutex>

#include "Defines.h"

class Queue
{
public:
    void Enqueue(const ItemPtr& mItem);
    ItemPtr Dequeue();

private:
    std::mutex mQueueMutex;
    ItemQueue mQueue;
};

