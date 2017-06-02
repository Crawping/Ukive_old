#include "UCommon.h"
#include "UTags.h"
#include "UMessage.h"
#include "WICManager.h"
#include "UWindowManager.h"
#include "UDeviceManager.h"
#include "UWindowClass.h"
#include "LodWindow.h"
#include "TestWindow.h"
#include "TextWindow.h"
#include "UMessageLooper.h"
#include "UTsfManager.h"
#include "UWordBreaker.h"
#include "UApplication.h"


int UApplication::mWindowIdCounter = 0;
int UApplication::mWidgetIdCounter = 0;
bool UApplication::mVSyncEnabled = true;


UApplication::UApplication()
{
	initApplication();
}

UApplication::UApplication(wchar_t *cl)
{
	parseCommandLine(cl);
	initApplication();
}

UApplication::UApplication(int argc, wchar_t *argv[])
{
	for (int i = 0; i < argc; ++i)
		mCommandList.push_back(argv[i]);
	initApplication();
}


UApplication::~UApplication()
{
	UMessageLooper::close();
	UMessage::close();
	UAnimationManager::closeGlobal();
	UWordBreaker::closeGlobal();

	if (mTsfManager)
	{
		mTsfManager->close();
		delete mTsfManager;
	}

	mDeviceManager->close();
	delete mDeviceManager;

	delete mWindowManager;

	mWICManager->close();
	delete mWICManager;

	::CoUninitialize();
}


void UApplication::parseCommandLine(wchar_t *cmdLine)
{
	if (cmdLine == nullptr)
		return;

	std::wstring cmdString = cmdLine;
	if (cmdString.empty())
		return;

	size_t i = cmdString.find(L" ");
	if (i == std::wstring::npos)
	{
		mCommandList.push_back(cmdString);
		return;
	}

	size_t newStart = 0;

	while (i != std::wstring::npos)
	{
		std::wstring tmp = cmdString.substr(newStart, i - newStart);
		mCommandList.push_back(tmp);

		newStart = i + 1;
		i = cmdString.find(L" ", newStart);
		if (i == std::wstring::npos)
		{
			tmp = cmdString.substr(newStart, cmdString.length() - newStart);
			mCommandList.push_back(tmp);
		}
	}
}


void UApplication::initApplication()
{
	HRESULT hr = E_FAIL;

	hr = ::CoInitialize(0);
	if (FAILED(hr))
		throw std::runtime_error(
			"UApplication-initApplication(): Init COM failed.");

	UMessage::init(50);
	UMessageLooper::init();
	UMessageLooper::prepareMainLooper();
	UWordBreaker::initGlobal();

	hr = UAnimationManager::initGlobal();
	if (FAILED(hr))
		throw std::runtime_error(
			"UAnimationManager-initGlobal(): Init anim library failed.");

	mAppQuitStrategy = QUIT_WHEN_STARTUP_WINDOW_CLOSED;

	mWindowIdCounter = WINDOW_ID_INIT;
	mWidgetIdCounter = WIDGET_ID_INIT;

	mWICManager = new WICManager();
	hr = mWICManager->init();
	if (FAILED(hr))
		throw std::runtime_error("UApplication-initApplication(): Init WIC failed.");

	mScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	mScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	mTsfManager = new UTsfManager();
	hr = mTsfManager->init();
	if (FAILED(hr))
		throw std::runtime_error("UApplication-initApplication(): Init Tsf failed.");

	mWindowClass = new UWindowClass(this);
	mWindowManager = new UWindowManager(this);

	mDeviceManager = new UDeviceManager();
	hr = mDeviceManager->init();
	if (FAILED(hr))
		throw std::runtime_error("UApplication-initApplication(): Init DirectX device failed.");


	int windowWidth = 800;
	int windowHeight = 500;

	int windowX = static_cast<int>((mScreenWidth - windowWidth) / 2.f);
	int windowY = static_cast<int>((mScreenHeight - windowHeight) / 2.f);

	//TODO: 有多个窗体时，在切换激活窗体时会出现渲染问题。
	UWindow *lodWindow = new LodWindow(this);
	lodWindow->setSize(windowWidth, windowHeight);
	lodWindow->setPosition(windowX, windowY);
	lodWindow->setAsStartupWindow();
	lodWindow->show();

	/*UWindow *d3dWindow = new TestWindow(this, UWindowId::MAIN_WINDOW);
	d3dWindow->setSize(windowWidth, windowHeight);
	d3dWindow->setPosition(windowX, windowY);
	d3dWindow->show();*/

	/*UWindow *textWindow = new TextWindow(this);
	textWindow->setSize(windowWidth, windowHeight);
	textWindow->setPosition(windowX, windowY);
	textWindow->show();*/
}


void UApplication::run()
{
	MSG msg;
	bool done = false;
	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		if (mVSyncEnabled)
			HRESULT hr = mDeviceManager->getCurOutput()->WaitForVBlank();

		UMessageLooper::loop();
		while (::PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
				UMessageLooper::myLooper()->quit();
				break;
			}
			else
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}
}


std::wstring UApplication::getCommand(size_t index)
{
	return mCommandList.at(index);
}


size_t UApplication::getCommandCount()
{
	return mCommandList.size();
}


void UApplication::setAppQuitStrategy(QuitStrategy qs)
{
	mAppQuitStrategy = qs;
}


UApplication::QuitStrategy UApplication::getAppQuitStrategy()
{
	return mAppQuitStrategy;
}

UWindowManager *UApplication::getWindowManager()
{
	return mWindowManager;
}

WICManager *UApplication::getWICManager()
{
	return mWICManager;
}

UWindowClass *UApplication::getWindowClass()
{
	return this->mWindowClass;
}

UDeviceManager *UApplication::getDeviceManager()
{
	return mDeviceManager;
}

UTsfManager *UApplication::getTsfManager()
{
	return mTsfManager;
}


int UApplication::getWindowUID()
{
	return mWindowIdCounter++;
}

int UApplication::getWidgetUID()
{
	return mWidgetIdCounter++;
}

void UApplication::setVSync(bool enable)
{
	mVSyncEnabled = enable;
}

bool UApplication::isVSyncEnabled()
{
	return mVSyncEnabled;
}


int UApplication::getScreenWidth()
{
	return mScreenWidth;
}

int UApplication::getScreenHeight()
{
	return mScreenHeight;
}