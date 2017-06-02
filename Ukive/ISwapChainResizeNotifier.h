#pragma once

class ISwapChainResizeNotifier
{
public:
	virtual void onSwapChainResize() = 0;
	virtual void onSwapChainResized() = 0;
};