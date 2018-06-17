#pragma once

#include "IConsumer.h"
#include "Defines.h"
#include "IQueueManager.h"

class Consumer : public IConsumer
{
public:
    explicit Consumer(IQueueManager* aQueueManager);

    Item Consume(const std::string& aQueueName, std::size_t aOffset) override;

private:
    IQueueManager* mQueueManager;
};

