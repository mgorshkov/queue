#pragma once

#include "Defines.h"

class Queue
{
public:
    Queue();

    void Enqueue(const DataType &aData);
    ItemPtr Dequeue(std::size_t aOffset);

private:
    ItemQueue mQueue;
    std::size_t mOffset;
};

