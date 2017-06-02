#include "UCommon.h"
#include "UApplication.h"
#include "UExecutable.h"
#include "UMessageLooper.h"
#include "USystemClock.h"
#include "UMessage.h"
#include "UCycler.h"


UCycler::UCycler()
{
	mLooper = UMessageLooper::myLooper();
}

UCycler::UCycler(UMessageLooper *looper)
{
	mLooper = looper;
}

UCycler::~UCycler()
{
	mLooper->getQueue()->remove(this, 0);
}


void UCycler::post(UExecutable *exec)
{
	this->postDelayed(exec, 0);
}

void UCycler::postDelayed(UExecutable *exec, ULONG64 millis)
{
	this->postAtTime(exec, millis + USystemClock::upTimeMillis());
}

void UCycler::postAtTime(UExecutable *exec, ULONG64 atTimeMillis)
{
	UMessage *msg = UMessage::obtain();
	msg->callback = exec;

	this->sendMessageAtTime(msg, atTimeMillis);
}

bool UCycler::hasCallbacks(UExecutable *exec)
{
	return mLooper->getQueue()->contains(this, exec, 0);
}

void UCycler::removeCallbacks(UExecutable *exec)
{
	mLooper->getQueue()->remove(this, exec, 0);
}


void UCycler::sendEmptyMessage(int what)
{
	this->sendEmptyMessageDelayed(what, 0);
}

void UCycler::sendEmptyMessageDelayed(int what, ULONG64 millis)
{
	this->sendEmptyMessageAtTime(what, millis + USystemClock::upTimeMillis());
}

void UCycler::sendEmptyMessageAtTime(int what, ULONG64 atTimeMillis)
{
	UMessage *msg = UMessage::obtain();
	msg->what = what;

	this->sendMessageAtTime(msg, atTimeMillis);
}

void UCycler::sendMessage(UMessage *msg)
{
	this->sendMessageDelayed(msg, 0);
}

void UCycler::sendMessageDelayed(UMessage *msg, ULONG64 millis)
{
	this->sendMessageAtTime(msg, millis + USystemClock::upTimeMillis());
}

void UCycler::sendMessageAtTime(UMessage *msg, ULONG64 atTimeMillis)
{
	msg->when = atTimeMillis;
	this->enqueueMessage(msg);
}


void UCycler::enqueueMessage(UMessage *msg)
{
	msg->target = this;
	mLooper->getQueue()->enqueue(msg);
}


bool UCycler::hasMessages(int what)
{
	return mLooper->getQueue()->contains(this, what, 0);
}

bool UCycler::hasMessages(int what, void *data)
{
	return mLooper->getQueue()->contains(this, what, data);
}

void UCycler::removeMessages(int what)
{
	mLooper->getQueue()->remove(this, what, 0);
}

void UCycler::removeMessages(int what, void *data)
{
	mLooper->getQueue()->remove(this, what, data);
}


void UCycler::dispatchMessage(UMessage *msg)
{
	if (msg->callback)
		msg->callback->run();
	else
		this->handleMessage(msg);
}

void UCycler::handleMessage(UMessage *msg)
{
}