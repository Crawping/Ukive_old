#pragma once

class UWindow;

class UWindowSlave
{
private:
	HWND mHandle;
	UWindow *mMaster;

	void initSlave();

public:
	UWindowSlave(UWindow *master);
	~UWindowSlave();

	void sync();

	HWND getWindowHandle();

	LRESULT CALLBACK messageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT processDWMProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool *pfCallDWP);
	LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam, int leftExt, int topExt, int rightExt, int bottomExt);
};

static UWindowSlave *sUWSVtr;
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);