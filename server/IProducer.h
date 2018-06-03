#pragma once

#include "Defines.h"

class IProducer
{
public:
    virtual ~IProducer() = default;
    virtual void Produce(const std::string& aQueueName, const DataType& aData) = 0;
};

