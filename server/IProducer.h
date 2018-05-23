#pragma once

#include "Item.h"

class IProducer
{
public:
	virtual ~IProducer() = default;
	virtual void Produce(ItemPtr aItem) = 0;
};

