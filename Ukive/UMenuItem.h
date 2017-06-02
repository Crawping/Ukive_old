#pragma once
#include "UCommon.h"

class UMenuItem
{
public:
	virtual void setItemTitle(std::wstring title) = 0;
	virtual void setItemVisible(bool visible) = 0;
	virtual void setItemEnabled(bool enable) = 0;

	virtual int getItemId() = 0;
	virtual std::int32_t getItemOrder() = 0;
	virtual std::wstring getItemTitle() = 0;
	virtual bool isItemVisible() = 0;
	virtual bool isItemEnabled() = 0;
};