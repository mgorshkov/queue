#include "stdafx.h"

#include <iostream>
#include <sstream>

ThreadedActor::ThreadedActor(Synchronizer& aSynchronizer, const std::string& aThreadPrefix, int aThreadCount)
	: mSynchronizer(aSynchronizer)
	, mThreadPrefix(aThreadPrefix)
	, mThreadCount(aThreadCount)
	, mIsRunning(false)
{
}

ThreadedActor::~ThreadedActor()
{
	if (mIsRunning)
		std::cerr << "~ThreadedActor on running threads" << std::endl;
}

void ThreadedActor::Start()
{
	for (int i = 0; i < mThreadCount; ++i)
	{
		std::stringstream name;
		name << mThreadPrefix << i;
		mThreads.emplace_back(ThreadProc, this, name.str());
	}
	mIsRunning = true;
}

void ThreadedActor::Join()
{
	if (!mIsRunning)
		return;
	for (auto& thread : mThreads)
		if (thread.joinable())
			thread.join();
	mIsRunning = false;
}

void ThreadedActor::ThreadProc(ThreadedActor* aThis, const std::string& aThreadPrefix)
{
	try
	{
		std::cout << "Thread " << aThreadPrefix << " started." << std::endl;
		aThis->Run();
		std::cout << "Thread " << aThreadPrefix << " stopped." << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Thread " << aThreadPrefix << " failed." << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

