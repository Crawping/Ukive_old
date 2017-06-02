#include "UCommon.h"
#include "UApplication.h"
#include "UWindow.h"
#include "UInputEvent.h"
#include "UWindowManager.h"


UWindowManager::UWindowManager(UApplication *app)
{
	mApplication = app;
	mStartupWindowId = UWINDOW_ID_NONE;
}


UWindowManager::~UWindowManager()
{
}


UApplication *UWindowManager::getApplication()
{
	return mApplication;
}


void UWindowManager::addWindow(UWindow *window)
{
	if (window == 0)
		return;

	mWindowList.push_back(std::shared_ptr<UWindow>(window));

	if (mWindowList.size() == 1)
		window->setAsStartupWindow();
}


bool UWindowManager::removeWindow(UWindow *window)
{
	if (window == 0)
		return false;

	return this->removeWindow(window->getId());
}


bool UWindowManager::removeWindow(int id)
{
	for (auto it = mWindowList.begin();
		it != mWindowList.end();)
	{
		if ((*it)->getId() == id)
		{
			it = mWindowList.erase(it);
			return true;
		}
		else
			++it;
	}

	return false;
}


UWindow* UWindowManager::getWindowByPos(std::size_t pos)
{
	return mWindowList.at(pos).get();
}


UWindow* UWindowManager::getWindowById(int id)
{
	for (auto it = mWindowList.begin();
		it != mWindowList.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			return (*it).get();
		}
	}

	return 0;
}


std::size_t UWindowManager::getWindowCount()
{
	return mWindowList.size();
}


bool UWindowManager::isContain(int id)
{
	for (auto it = mWindowList.begin();
		it != mWindowList.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			return true;
		}
	}

	return false;
}


void UWindowManager::setStartupWindow(int id)
{
	mStartupWindowId = id;
}


int UWindowManager::getStartupWindow()
{
	return mStartupWindowId;
}