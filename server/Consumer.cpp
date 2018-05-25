#include "stdafx.h"

Consumer::Consumer(IStopperPtr aStopper)
	: mStopper(aStopper)
{
}

void Consumer::Consume(TaskPtr&& aTask)
{
	Original::ProcessRequest(aTask->mRequest, mStopper);
}

