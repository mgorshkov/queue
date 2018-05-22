#include "stdafx.h"

Producer::Producer(IStopperPtr aStopper)
	: mStopper(aStopper)
{
}

TaskPtr Producer::Produce(const std::string& aStr) const
{
	return std::make_unique<Task>(aStr);
}

