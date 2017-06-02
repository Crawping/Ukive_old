#include "UCommon.h"
#include "UMessage.h"
#include "USystemClock.h"
#include "UMessageQueue.h"


UMessageQueue::UMessageQueue()
{
	mMessage = nullptr;
	mQuitting = false;
	mHasBarrier = false;
}


UMessageQueue::~UMessageQueue()
{
}


void UMessageQueue::quit()
{
	std::lock_guard<std::mutex> lk(mQueueSync);

	mQuitting = true;
	this->removeAllLocked();
}


bool UMessageQueue::enqueue(UMessage *msg)
{
	if (!msg->target)
		throw std::logic_error(
			"UMessageQueue-enqueue(): Message must have a target.");

	std::lock_guard<std::mutex> lk(mQueueSync);

	if (mQuitting)
	{
		msg->recycle();
		return false;
	}

	UMessage *ptr = mMessage;

	if (!ptr || !ptr->target 
		|| msg->when == 0 || msg->when < ptr->when)
	{
		msg->next = ptr;
		mMessage = msg;
	}
	else
	{
		UMessage *prev = 0;
		while (true)
		{
			prev = ptr;
			ptr = ptr->next;
			if (!ptr || !ptr->target 
				|| msg->when < ptr->when)
				break;
		}

		msg->next = ptr;
		prev->next = msg;
	}

	return true;
}

UMessage *UMessageQueue::dequeue()
{
	std::lock_guard<std::mutex> lk(mQueueSync);

	ULONG64 now = USystemClock::upTimeMillis();
	UMessage *prev = 0;
	UMessage *ptr = mMessage;

	//find barrier.
	while (ptr && ptr->target)
	{
		prev = ptr;
		ptr = ptr->next;
	}

	if (ptr == 0)
		throw std::logic_error("UMessageQueue-dequeue(): cannot find barrier.");

	//over the barrier.
	prev = ptr;
	ptr = ptr->next;

	while (ptr)
	{
		if (ptr->when <= now)
		{
			prev->next = ptr->next;
			UMessage *msg = ptr;
			return msg;
		}

		prev = ptr;
		ptr = ptr->next;
	}

	return 0;
}


void UMessageQueue::remove(UCycler *c, void *data)
{
	if (c == 0)
		return;

	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	UMessage *prev = 0;
	while (ptr)
	{
		if (ptr->target == c
			&& (data == 0 || ptr->data == data))
		{
			if (prev)
				prev->next = ptr->next;
			else
				mMessage = ptr->next;

			UMessage *msg = ptr;
			ptr = ptr->next;

			msg->recycle();
			continue;
		}

		prev = ptr;
		ptr = ptr->next;
	}
}

void UMessageQueue::remove(UCycler *c, int what, void *data)
{
	if (c == 0)
		return;

	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	UMessage *prev = 0;
	while (ptr)
	{
		if (ptr->target == c
			&& ptr->what == what
			&& (data == 0 || ptr->data == data))
		{
			if (prev)
				prev->next = ptr->next;
			else
				mMessage = ptr->next;

			UMessage *msg = ptr;
			ptr = ptr->next;

			msg->recycle();
			continue;
		}

		prev = ptr;
		ptr = ptr->next;
	}
}

void UMessageQueue::remove(UCycler *c, UExecutable *exec, void *data)
{
	if (c == 0)
		return;

	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	UMessage *prev = 0;
	while (ptr)
	{
		if (ptr->target == c 
			&& ptr->callback == exec 
			&& (data == 0 || ptr->data == data))
		{
			if (prev)
				prev->next = ptr->next;
			else
				mMessage = ptr->next;

			UMessage *msg = ptr;
			ptr = ptr->next;

			msg->recycle();
			continue;
		}

		prev = ptr;
		ptr = ptr->next;
	}
}

void UMessageQueue::removeAllLocked()
{
	UMessage *ptr = mMessage;
	UMessage *msg = 0;
	while (ptr)
	{
		msg = ptr;
		ptr = ptr->next;
		msg->recycle();
	}

	mMessage = 0;
}


bool UMessageQueue::contains(UCycler *c, int what, void *data)
{
	if (c == 0)
		return false;

	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	while (ptr)
	{
		if (ptr->target == c
			&& ptr->what == what
			&& (data == 0 || ptr->data == data))
			return true;
		ptr = ptr->next;
	}

	return false;
}

bool UMessageQueue::contains(UCycler *c, UExecutable *exec, void *data)
{
	if (c == 0)
		return false;

	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	while (ptr)
	{
		if (ptr->target == c 
			&& ptr->callback == exec 
			&& (data == 0 || ptr->data == data))
			return true;
		ptr = ptr->next;
	}

	return false;
}


void UMessageQueue::addBarrier()
{
	std::lock_guard<std::mutex> lk(mQueueSync);

	if (mHasBarrier)
		return;

	UMessage *barrier = UMessage::obtain();
	if (mMessage)
		barrier->next = mMessage;
	mMessage = barrier;

	mHasBarrier = true;
}

void UMessageQueue::removeBarrier()
{
	std::lock_guard<std::mutex> lk(mQueueSync);

	UMessage *ptr = mMessage;
	UMessage *prev = 0;
	while (ptr)
	{
		if (ptr->target == 0)
		{
			if (prev)
				prev->next = ptr->next;
			else
				mMessage = ptr->next;

			UMessage *msg = ptr;
			ptr = ptr->next;

			msg->recycle();
			break;
		}

		prev = ptr;
		ptr = ptr->next;
	}

	mHasBarrier = false;
}