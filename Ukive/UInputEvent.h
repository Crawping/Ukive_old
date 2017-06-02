#pragma once

class UInputEvent
{
private:
	int mMouseX;
	int mMouseY;
	int mMouseRawX;
	int mMouseRawY;
	int mMouseWheel;
	int mMouseKey;

	int mKeyboardVirtualKey;

	int mEventType;

	bool mIsOutside;
	bool mIsMouseCaptured;

public:
	//鼠标事件。
	static const int EVENT_MOUSE_KEY_DOWN = 1;
	static const int EVENT_MOUSE_KEY_UP = 2;
	static const int EVENT_MOUSE_MOVE = 3;
	static const int EVENT_MOUSE_WHEEL = 4;
	static const int EVENT_MOUSE_LEAVE_WINDOW = 5;
	static const int EVENT_MOUSE_LEAVE = 6;
	static const int EVENT_MOUSE_HOVER = 7;
	static const int EVENT_MOUSE_SCROLL_ENTER = 8;
	
	//键盘事件。
	static const int EVENT_KEYBOARD_KEY_DOWN = 10;
	static const int EVENT_KEYBOARD_KEY_UP = 11;

	//其他事件。
	static const int EVENT_CANCEL = 20;

	//鼠标按键定义。
	static const int KEY_MOUSE_LEFT = 100;
	static const int KEY_MOUSE_MIDDLE = 101;
	static const int KEY_MOUSE_RIGHT = 102;

	//键盘按键定义使用windows本身的定义。

public:
	UInputEvent();
	UInputEvent(UInputEvent *source);
	~UInputEvent();

	void setEvent(int eventType);
	void setMouseX(int mouseX);
	void setMouseY(int mouseY);
	void setMouseRawX(int rawX);
	void setMouseRawY(int rawY);
	void setMouseWheel(int wheel);
	void setMouseKey(int key);
	void setKeyboardKey(int virtualKey, int exMsg);

	void setOutside(bool outside);
	void setIsMouseCaptured(bool captured);

	int getEvent();
	int getMouseX();
	int getMouseY();
	int getMouseRawX();
	int getMouseRawY();
	int getMouseWheel();
	int getMouseKey();
	int getKeyboardKey();

	bool isMouseEvent();
	bool isKeyboardEvent();
	bool isMouseCaptured();

	/// <summary>
	/// 当鼠标事件发生于Widget外部时，该方法返回true。
	/// 只有当Widget的setReceiveOutsideInputEvent()方法以true为参数
	/// 调用之后，此方法才有效。
	/// </summary>
	bool isOutside();
};

