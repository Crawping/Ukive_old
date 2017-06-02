#include "UCommon.h"
#include "UCycler.h"
#include "UMessage.h"
#include "UApplication.h"
#include "UMessageLooper.h"


std::mutex UMessageLooper::mLooperSync;
UMessageLooper *UMessageLooper::mMainLooper = nullptr;
UThreadLocal<std::shared_ptr<UMessageLooper>> *UMessageLooper::mThreadLocal = nullptr;


UMessageLooper::UMessageLooper()
{
	mMsgQueue = new UMessageQueue();
}

UMessageLooper::~UMessageLooper()
{
	delete mMsgQueue;
}


void UMessageLooper::init()
{
	if (mThreadLocal)
		throw std::logic_error("UMessageLooper-init(): can only init once.");

	mThreadLocal = new UThreadLocal<std::shared_ptr<UMessageLooper>>();
}

void UMessageLooper::close()
{
	if (mThreadLocal)
	{
		delete mThreadLocal;
		mThreadLocal = nullptr;
	}
}


void UMessageLooper::quit()
{
	mMsgQueue->quit();
}

UMessageQueue *UMessageLooper::getQueue()
{
	return mMsgQueue;
}


void UMessageLooper::prepare()
{
	std::lock_guard<std::mutex> lk(mLooperSync);

	std::shared_ptr<UMessageLooper> looperPtr;
	if (mThreadLocal->get(looperPtr))
		throw std::logic_error("UMessageLooper-prepare(): Only one Looper may be created per thread");

	std::shared_ptr<UMessageLooper> newLooper(new UMessageLooper());
	mThreadLocal->set(newLooper);
}

void UMessageLooper::prepareMainLooper()
{
	prepare();
	std::lock_guard<std::mutex> lk(mLooperSync);

	if (mMainLooper)
		throw std::logic_error("The main Looper has already been prepared.");
	mMainLooper = myLooper();
}

void UMessageLooper::loop()
{
	UMessageLooper *looper = myLooper();
	if (!looper)
		throw std::logic_error(
			"UMessageLooper-loop(): No Looper; Looper.prepare() wasn't called on this thread.");

	UMessageQueue *queue = looper->getQueue();

	queue->addBarrier();

	while (true)
	{
		UMessage *msg = queue->dequeue();
		if (!msg)
			break;

		msg->target->dispatchMessage(msg);
		msg->recycle();
	}

	queue->removeBarrier();
}

UMessageQueue *UMessageLooper::myQueue()
{
	return myLooper()->getQueue();
}

UMessageLooper *UMessageLooper::myLooper()
{
	std::shared_ptr<UMessageLooper> looperPtr;
	if (mThreadLocal->get(looperPtr))
		return looperPtr.get();

	return nullptr;
}

UMessageLooper *UMessageLooper::getMainLooper()
{
	std::lock_guard<std::mutex> lk(mLooperSync);
	return mMainLooper;
}


void UMessageLooper::receiveMessage()
{
	mMsgQueue->addBarrier();

	while (true)
	{
		UMessage *msg = mMsgQueue->dequeue();
		if (msg == 0)
			break;

		msg->target->dispatchMessage(msg);
	}

	mMsgQueue->removeBarrier();
}