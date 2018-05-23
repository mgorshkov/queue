#include "Producer.h"

Producer::Producer()
{
}

void Producer::Produce(ItemPtr aItem) const
{
	return std::make_unique<Item>(aItem);
}

