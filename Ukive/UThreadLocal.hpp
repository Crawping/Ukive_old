#pragma once
#include "UCommon.h"

template<class T>
class UThreadLocal
{
private:
	static thread_local std::list<T> sValueList;

public:
	UThreadLocal();
	~UThreadLocal();

	void set(T value);
	bool get(T &value);
	void remove();
};


template<class T>
thread_local std::list<T> UThreadLocal<T>::sValueList;


template<class T>
UThreadLocal<T>::UThreadLocal()
{
}

template<class T>
UThreadLocal<T>::~UThreadLocal()
{
}


template<class T>
void UThreadLocal<T>::set(T value)
{
	sValueList.push_front(value);
}

template<class T>
bool UThreadLocal<T>::get(T &value)
{
	if (sValueList.empty())
		return false;
	value = sValueList.front();
	return true;
}

template<class T>
void UThreadLocal<T>::remove()
{
	sValueList.clear();
}