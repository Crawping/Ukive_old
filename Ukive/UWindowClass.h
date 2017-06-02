#pragma once

class UWindow;
class UInputEvent;

class UWindowClass
{
private:
	std::wstring mWindowClassName;
	WNDCLASSEXW mWindowClassNative;

	UInputEvent *mInputEvent;
	UApplication *mApplication;

	bool createWindowClass(std::wstring className);
	void processInputEvent(UWindow *window, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	UWindowClass(UApplication *app);
	~UWindowClass();

	std::wstring getWindowClassName();

	LRESULT CALLBACK messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT processDWMProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool *pfCallDWP);
	LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam, int leftExt, int topExt, int rightExt, int bottomExt);
};


static UWindowClass *vtr;
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);