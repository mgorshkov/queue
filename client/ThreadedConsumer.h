#pragma once

template <typename DependentConsumer>
class ThreadedConsumer : public ThreadedActor
{
public:
	template <typename... Args>
	ThreadedConsumer(Args&&... args)
		: ThreadedActor(std::forward<Args>(args)...)
	{
	}

protected:
	void Run() override
	{
		DependentConsumer dependentConsumer(mSynchronizer.mStopper);
		while (!mSynchronizer.IsStopped())
		{
			mSynchronizer.Wait();
			if (!mSynchronizer.IsStopped())
				ProcessQueue(dependentConsumer);
		}
	}

	void ProcessQueue(IConsumer& aDependentConsumer)
	{
		TaskQueue queue = mSynchronizer.GetQueue();
		while (!queue.empty())
		{
			aDependentConsumer.Consume(std::move(queue.front()));
			queue.pop();
		}
	}
};

