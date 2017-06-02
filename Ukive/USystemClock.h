#pragma once

class USystemClock
{
public:
	USystemClock();
	~USystemClock();

	static ULONG64 upTimeMillis();
};