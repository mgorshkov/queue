#pragma once

#include "IProducer.h"
#include "IQueueManager.h"

class Producer : public IProducer
{
public:
    explicit Producer(IQueueManager* aQueueManager);

    bool Produce(const std::string& aQueueName, const DataType& aData) override;

private:
    IQueueManager* mQueueManager;
};

