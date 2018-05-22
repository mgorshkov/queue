#pragma once

template <typename DependentProducer>
class ThreadedProducer : public ThreadedActor
{
public:
	template <typename... Args>
	ThreadedProducer(Args&&... args)
		: ThreadedActor(std::forward<Args>(args)...)
	{
	}
	
protected:
	void Run() override
	{
		DependentProducer dependentProducer(mSynchronizer.mStopper);
		while (!mSynchronizer.IsStopped())
		{
			TaskPtr task = dependentProducer.Produce();
			if (!task->IsValid())
				break; // stop has been detected inside producer

			mSynchronizer.EnqueueTask(std::move(task));
		}
	}
};

