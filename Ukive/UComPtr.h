#pragma once

template <class T>
class UComPtr
{
private:
	T *mPtr;

public:
	UComPtr()
	{
		mPtr = nullptr;
	}

	UComPtr(T *real)
	{
		mPtr = real;
	}

	UComPtr(const UComPtr& rhs)
	{
		if (rhs.mPtr)
			rhs.mPtr->AddRef();
		mPtr = rhs.mPtr;
	}

	~UComPtr()
	{
		reset();
	}


	UComPtr& operator =(const UComPtr& rhs)
	{
		//防止自身给自身赋值。
		if (this == (UComPtr*)(&(int&)rhs))
			return *this;

		if (rhs.mPtr)
			rhs.mPtr->AddRef();

		if (mPtr)
			mPtr->Release();
		mPtr = rhs.mPtr;
		return *this;
	}

	UComPtr& operator =(T *real)
	{
		if (real == mPtr)
			return *this;

		if (mPtr)
			mPtr->Release();
		mPtr = real;
		return *this;
	}

	T* operator ->()
	{
		return mPtr;
	}

	T** operator &()
	{
		return (&mPtr);
	}

	bool operator ==(nullptr_t)
	{
		return (mPtr == nullptr);
	}

	bool operator !=(nullptr_t)
	{
		return (mPtr != nullptr);
	}

	bool operator !()
	{
		return (mPtr == nullptr);
	}

	T* get()
	{
		return mPtr;
	}

	template<class Ct>
	UComPtr<Ct> cast()
	{
		Ct *casted = nullptr;
		mPtr->QueryInterface<Ct>(&casted);
		return UComPtr<Ct>(casted);
	}

	void reset()
	{
		if (mPtr)
		{
			mPtr->Release();
			mPtr = nullptr;
		}
	}
};