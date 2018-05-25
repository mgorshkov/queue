#pragma once

#include "IConsumer.h"
#include "IStopper.h"
#include "Task.h"

class Consumer : public IConsumer
{
public:
	explicit Consumer(IStopperPtr aStopper);

	void Consume(TaskPtr&& aTask) override;

private:
	IStopperPtr mStopper;
};

