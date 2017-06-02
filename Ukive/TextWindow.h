#pragma once
#include "UWindow.h"

class TextWindow : public UWindow
{
private:
	static const uint32_t ID_TV_EDITOR = 0x1;
	static const uint32_t ID_LAYOUT_ROOT = 0x2;

public:
	//使用基类的构造函数。
	using UWindow::UWindow;

	virtual void onCreate() override;
	virtual bool onMoving(RECT *rect) override;
	virtual bool onResizing(WPARAM edge, RECT *rect) override;
	virtual void onMove(int x, int y) override;
	virtual void onResize(
		int param, int width, int height,
		int clientWidth, int clientHeight) override;
};