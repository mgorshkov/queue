#pragma once

#include "Defines.h"

class IConsumer
{
public:
	virtual ~IConsumer() = default;
    virtual Item Consume(std::size_t aOffset) = 0;
};
