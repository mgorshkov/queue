#pragma once

#include "Defines.h"

class IProducer
{
public:
    virtual ~IProducer() = default;
    virtual bool Produce(const std::string& aQueueName, const DataType& aData) = 0;
};

