#include "Consumer.h"

Consumer::Consumer()
{
}

void Consumer::Consume(ItemPtr&& aItem)
{
	Original::ProcessRequest(aTask->mRequest, mStopper);
}

