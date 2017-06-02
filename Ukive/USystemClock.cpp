#include "UCommon.h"
#include "USystemClock.h"


USystemClock::USystemClock()
{
}


USystemClock::~USystemClock()
{
}


ULONG64 USystemClock::upTimeMillis()
{
	return ::GetTickCount64();
}