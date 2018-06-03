#pragma once

#include "IConsumer.h"
#include "Defines.h"
#include "Queue.h"

class Consumer : public IConsumer
{
public:
    explicit Consumer(Queue& aQueue);

    ItemPtr Consume(std::size_t aOffset) override;

private:
    Queue& mQueue;
};

