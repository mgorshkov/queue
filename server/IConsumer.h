#pragma once

#include "Defines.h"

class IConsumer
{
public:
	virtual ~IConsumer() = default;
	virtual ItemPtr Consume();
};
