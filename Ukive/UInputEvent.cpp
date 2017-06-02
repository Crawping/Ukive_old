#include "UInputEvent.h"


UInputEvent::UInputEvent()
{
	mIsOutside = false;
	mIsMouseCaptured = false;
}

UInputEvent::UInputEvent(UInputEvent *source)
{
	mMouseX = source->mMouseX;
	mMouseY = source->mMouseY;
	mMouseRawX = source->mMouseRawX;
	mMouseRawY = source->mMouseRawY;
	mMouseWheel = source->mMouseWheel;
	mMouseKey = source->mMouseKey;
	mKeyboardVirtualKey = source->mKeyboardVirtualKey;
	mEventType = source->mEventType;
	mIsOutside = source->mIsOutside;
	mIsMouseCaptured = source->mIsMouseCaptured;
}


UInputEvent::~UInputEvent()
{
}

void UInputEvent::setEvent(int eventType)
{
	mEventType = eventType;
}

void UInputEvent::setMouseX(int mouseX)
{
	mMouseX = mouseX;
}

void UInputEvent::setMouseY(int mouseY)
{
	mMouseY = mouseY;
}

void UInputEvent::setMouseRawX(int rawX)
{
	mMouseRawX = rawX;
}

void UInputEvent::setMouseRawY(int rawY)
{
	mMouseRawY = rawY;
}

void UInputEvent::setMouseWheel(int wheel)
{
	mMouseWheel = wheel;
}

void UInputEvent::setMouseKey(int key)
{
	mMouseKey = key;
}

void UInputEvent::setKeyboardKey(int virtualKey, int exMsg)
{
	int curSlot = exMsg;
	//For WM_KEYDOWN:
	//The repeat count for the current message. 
	//The value is the number of times the keystroke is autorepeated as a result of the user holding down the key. 
	//If the keystroke is held long enough, multiple messages are sent. However, the repeat count is not cumulative.
	//
	//For WM_KEYUP:
	//The repeat count for the current message. 
	//The value is the number of times the keystroke is autorepeated as a result of the user holding down the key. 
	//The repeat count is always 1 for a WM_KEYUP message.
	int repeatCount = curSlot & 0x0000ffff;  //0-15bit

	curSlot = (curSlot & 0xffff0000) >> 16;
	//The scan code. The value depends on the OEM.
	int scanCode = curSlot & 0x00ff;         //16-23bit

	curSlot = (curSlot & 0xff00) >> 8;
	//Indicates whether the key is an extended key, 
	//such as the right-hand ALT and CTRL keys that appear on an enhanced 101- or 102-key keyboard. 
	//The value is 1 if it is an extended key; otherwise, it is 0.
	int isExtendKey = curSlot & 0x01;        //24bit

	curSlot = (curSlot & 0xfe) >> 1;
	//Reserved; do not use.
	int reserved = curSlot & 0x0f;           //25-28bit

	curSlot = (curSlot & 0xf0) >> 4;
	//The context code. The value is always 0 for a WM_KEYDOWN message.
	//The context code. The value is always 0 for a WM_KEYUP message.
	int contextCode = curSlot & 0x01;        //29bit

	curSlot = (curSlot & 0xfe) >> 1;
	//The previous key state. 
	//The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
	//The previous key state. The value is always 1 for a WM_KEYUP message.
	int prevKeyState = curSlot & 0x01;       //30bit

	curSlot = (curSlot & 0xfe) >> 1;
	//The transition state. The value is always 0 for a WM_KEYDOWN message.
	//The transition state. The value is always 1 for a WM_KEYUP message.
	int transitionState = curSlot;           //31bit

	mKeyboardVirtualKey = virtualKey;
}

void UInputEvent::setOutside(bool outside)
{
	mIsOutside = outside;
}

void UInputEvent::setIsMouseCaptured(bool captured)
{
	mIsMouseCaptured = captured;
}


int UInputEvent::getEvent()
{
	return mEventType;
}

int UInputEvent::getMouseX()
{
	return mMouseX;
}

int UInputEvent::getMouseY()
{
	return mMouseY;
}

int UInputEvent::getMouseRawX()
{
	return mMouseRawX;
}

int UInputEvent::getMouseRawY()
{
	return mMouseRawY;
}

int UInputEvent::getMouseWheel()
{
	return mMouseWheel;
}

int UInputEvent::getMouseKey()
{
	return mMouseKey;
}

int UInputEvent::getKeyboardKey()
{
	return mKeyboardVirtualKey;
}


bool UInputEvent::isMouseEvent()
{
	return (mEventType == EVENT_MOUSE_KEY_DOWN
		|| mEventType == EVENT_MOUSE_KEY_UP
		|| mEventType == EVENT_MOUSE_MOVE
		|| mEventType == EVENT_MOUSE_WHEEL
		|| mEventType == EVENT_MOUSE_LEAVE_WINDOW
		|| mEventType == EVENT_MOUSE_SCROLL_ENTER
		|| mEventType == EVENT_MOUSE_LEAVE
		|| mEventType == EVENT_MOUSE_HOVER);
}

bool UInputEvent::isKeyboardEvent()
{
	return (mEventType == EVENT_KEYBOARD_KEY_DOWN
		|| mEventType == EVENT_KEYBOARD_KEY_UP);
}

bool UInputEvent::isMouseCaptured()
{
	return mIsMouseCaptured;
}

bool UInputEvent::isOutside()
{
	return mIsOutside;
}