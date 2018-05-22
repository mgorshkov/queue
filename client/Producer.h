#pragma once

#include "IProducer.h"
#include "IStopper.h"
#include "Task.h"

class Producer : public IProducer
{
public:
	explicit Producer(IStopperPtr aStopper);

	TaskPtr Produce() const override;

private:
	IStopperPtr mStopper;
};

