#pragma once
#include "UCommon.h"

class UMenuItem;
class UMenuCallback;

class UMenu
{
public:
	virtual void setCallback(UMenuCallback *callback) = 0;
	virtual UMenuCallback *getCallback() = 0;

	virtual UMenuItem *addItem(int id, std::int32_t order, std::wstring title) = 0;
	virtual bool removeItem(int id) = 0;
	virtual bool hasItem(int id) = 0;
	virtual UMenuItem *findItem(int id) = 0;
	virtual std::size_t getItemCount() = 0;
};