#pragma once
#include "UMessageQueue.h"

class UApplication;
class UExecutable;
class UMessage;
class UMessageLooper;

class UCycler
{
private:
	UMessageLooper *mLooper;

public:
	UCycler();
	UCycler(UMessageLooper *looper);
	virtual ~UCycler();

	void post(UExecutable *exec);
	void postDelayed(UExecutable *exec, ULONG64 millis);
	void postAtTime(UExecutable *exec, ULONG64 atTimeMillis);

	bool hasCallbacks(UExecutable *exec);
	void removeCallbacks(UExecutable *exec);

	void sendMessage(UMessage *msg);
	void sendMessageDelayed(UMessage *msg, ULONG64 millis);
	void sendMessageAtTime(UMessage *msg, ULONG64 atTimeMillis);
	void sendEmptyMessage(int what);
	void sendEmptyMessageDelayed(int what, ULONG64 millis);
	void sendEmptyMessageAtTime(int what, ULONG64 atTimeMillis);

	void enqueueMessage(UMessage *msg);

	bool hasMessages(int what);
	bool hasMessages(int what, void *data);
	void removeMessages(int what);
	void removeMessages(int what, void *data);

	void dispatchMessage(UMessage *msg);
	virtual void handleMessage(UMessage *msg);
};