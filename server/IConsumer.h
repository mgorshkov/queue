#pragma once

#include "Item.h"

class IConsumer
{
public:
	virtual ~IConsumer() = default;
	virtual void Consume(ItemPtr&& aItem) = 0;
};
