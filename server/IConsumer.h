#pragma once

#include "Item.h"

class IConsumer
{
public:
	virtual ~IConsumer() = default;
	virtual ItemPtr Consume();
};
