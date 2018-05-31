#pragma once

#include "IProducer.h"
#include "Queue.h"

class Producer : public IProducer
{
public:
    explicit Producer(Queue& aQueue);

    void Produce(ItemPtr aItem) override;

private:
    Queue& mQueue;
};

