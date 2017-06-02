#pragma once
#include "UMessageQueue.h"
#include "UThreadLocal.hpp"

class UCycler;
class UMessage;

class UMessageLooper
{
private:
	UMessageLooper();

	UMessageQueue *mMsgQueue;

	static std::mutex mLooperSync;
	static UMessageLooper *mMainLooper;
	static UThreadLocal<std::shared_ptr<UMessageLooper>> *mThreadLocal;
	
public:
	~UMessageLooper();

	static void init();
	static void close();

	void quit();
	UMessageQueue *getQueue();

	static void prepare();
	static void prepareMainLooper();
	static void loop();
	static UMessageQueue *myQueue();
	static UMessageLooper *myLooper();
	static UMessageLooper *getMainLooper();
	
	void receiveMessage();
};