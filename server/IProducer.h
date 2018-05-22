#pragma once

#include "Item.h"

class IProducer
{
public:
	virtual ~IProducer() = default;
	virtual ItemPtr Produce() const = 0;
};

