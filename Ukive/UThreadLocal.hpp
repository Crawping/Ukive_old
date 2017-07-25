#pragma once
#include "UCommon.h"

template<class T>
class UThreadLocal
{
private:
	static std::map<DWORD, T> sValueList;

public:
	UThreadLocal()
	{
	}

	~UThreadLocal()
	{
	}

	void set(T value)
	{
		sValueList.insert(
			std::pair<DWORD, T>(
				::GetCurrentThreadId(), value));
	}

	bool get(T &value)
	{
		auto it = sValueList.find(::GetCurrentThreadId());
		if (it == sValueList.end())
			return false;
		else
			value = it->second;

		return true;
	}

	void remove()
	{
		sValueList.erase(::GetCurrentThreadId());
	}
};

template<class T>
std::map<DWORD, T> UThreadLocal<T>::sValueList;