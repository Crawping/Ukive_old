#include "UCommon.h"
#include "UMessage.h"


UMessage *UMessage::sPool = 0;
ULONG64 UMessage::sPoolSize = 0;
bool UMessage::mIsInited = false;
std::mutex UMessage::mPoolSync;


UMessage::UMessage()
{
	what = -1;
	when = 0;
	target = 0;
	callback = 0;
	data = 0;
	next = 0;
}

UMessage::~UMessage()
{
}


void UMessage::init(ULONG64 poolCap)
{
	if (mIsInited)
		throw std::logic_error("UMessage-init(): can only init once.");

	UMessage *ptr = 0;

	for (ULONG64 i = 0; i < poolCap; ++i)
	{
		UMessage *msg = new UMessage();
		if (sPool)
		{
			ptr->next = msg;
			ptr = msg;
		}
		else
		{
			sPool = msg;
			ptr = sPool;
		}
	}

	mIsInited = true;
	sPoolSize = poolCap;
}

void UMessage::close()
{
	while (sPool)
	{
		UMessage *msg = sPool;
		sPool = msg->next;
		delete msg;
	}

	mIsInited = false;
	sPoolSize = 0;
}


UMessage *UMessage::obtain()
{
	std::lock_guard<std::mutex> lk(mPoolSync);

	if (sPool)
	{
		UMessage *msg = sPool;
		sPool = msg->next;
		msg->next = 0;
		--sPoolSize;

		return msg;
	}

	return new UMessage();
}

void UMessage::recycle()
{
	std::lock_guard<std::mutex> lk(mPoolSync);

	this->next = sPool;
	sPool = this;
	++sPoolSize;

	what = -1;
	when = 0;
	target = 0;
	callback = 0;
	data = 0;
}