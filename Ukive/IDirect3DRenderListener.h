#pragma once

class IDirect3DRenderListener
{
public:
	virtual void onClear() = 0;
	virtual void onRender() = 0;
};