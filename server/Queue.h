#pragma once

#include "Defines.h"
#include "QueueStorage.h"

class Queue
{
public:
    Queue();

    void Load(const std::string& aFileName);

    void Enqueue(const DataType &aData);
    ItemPtr Dequeue(std::size_t aOffset);

private:
    ItemQueue mQueue;
    std::size_t mOffset;
    QueueStorage mStorage;
};

