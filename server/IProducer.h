#pragma once

#include "Defines.h"

class IProducer
{
public:
    virtual ~IProducer() = default;
    virtual void Produce(ItemPtr aItem) = 0;
};

