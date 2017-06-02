#pragma once

class UCycler;
class UExecutable;

class UMessage
{
public:
	int what;
	ULONG64 when;
	UCycler *target;
	UExecutable *callback;
	void *data;

	UMessage *next;

public:
	static UMessage *sPool;
	static ULONG64 sPoolSize;

public:
	static UMessage *obtain();
	void recycle();

	static void init(ULONG64 poolCap);
	static void close();

private:
	UMessage();
	~UMessage();

	static bool mIsInited;
	static std::mutex mPoolSync;
};