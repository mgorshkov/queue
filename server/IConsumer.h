#pragma once

#include "Defines.h"

class IConsumer
{
public:
	virtual ~IConsumer() = default;
    virtual ItemPtr Consume(std::size_t aOffset) = 0;
};
