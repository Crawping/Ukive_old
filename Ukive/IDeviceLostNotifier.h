#pragma once

class IDeviceLostNotifier
{
public:
	virtual void onDeviceLost() = 0;
	virtual void onDeviceRestored() = 0;
};