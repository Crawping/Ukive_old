#pragma once

#define WINDOW_ID_INIT  0x100
#define WIDGET_ID_INIT  0x100

class WICManager;
class UTsfManager;
class UWindowClass;
class UIDGenerator;
class UDeviceManager;
class UWindowManager;
class UMessageLooper;


class UApplication
{
public:
	enum QuitStrategy
	{
		QUIT_WHEN_LAST_WINDOW_CLOSED,
		QUIT_WHEN_STARTUP_WINDOW_CLOSED,
	};

private:
	WICManager *mWICManager;
	UTsfManager *mTsfManager;
	UWindowClass *mWindowClass;
	UWindowManager *mWindowManager;
	UDeviceManager *mDeviceManager;

	static int mWindowIdCounter;
	static int mWidgetIdCounter;

	static bool mVSyncEnabled;

	int mScreenWidth;
	int mScreenHeight;
	QuitStrategy mAppQuitStrategy;
	std::vector<std::wstring> mCommandList;

	void initApplication();
	void parseCommandLine(wchar_t *cmdLine);

public:
	UApplication();
	UApplication(wchar_t *cl);
	UApplication(int argc, wchar_t *argv[]);
	~UApplication();

	std::wstring getCommand(size_t index);
	size_t getCommandCount();

	WICManager *getWICManager();
	UWindowClass *getWindowClass();
	UDeviceManager *getDeviceManager();
	UWindowManager *getWindowManager();
	UTsfManager *getTsfManager();

	int getScreenWidth();
	int getScreenHeight();

	void setAppQuitStrategy(QuitStrategy qs);

	QuitStrategy getAppQuitStrategy();

	static int getWindowUID();
	static int getWidgetUID();

	static void setVSync(bool enable);
	static bool isVSyncEnabled();

	void run();
};

