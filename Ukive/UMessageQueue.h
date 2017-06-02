#pragma once

class UCycler;
class UMessage;
class UExecutable;

class UMessageQueue
{
private:
	bool mQuitting;
	bool mHasBarrier;
	UMessage *mMessage;
	std::mutex mQueueSync;

	void removeAllLocked();

public:
	UMessageQueue();
	~UMessageQueue();

	void quit();

	bool enqueue(UMessage *msg);
	UMessage *dequeue();
	void remove(UCycler *c, void *data);
	void remove(UCycler *c, int what, void *data);
	void remove(UCycler *c, UExecutable *exec, void *data);
	bool contains(UCycler *c, int what, void *data);
	bool contains(UCycler *c, UExecutable *exec, void *data);

	void addBarrier();
	void removeBarrier();
};