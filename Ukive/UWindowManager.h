#pragma once

class UWindow;
class UApplication;

class UWindowManager
{
protected:
	int mStartupWindowId;

	UApplication *mApplication;
	std::vector<std::shared_ptr<UWindow>> mWindowList;
	
public:
	UWindowManager(UApplication *app);
	virtual ~UWindowManager();

	static const int UWINDOW_ID_NONE = -1;

	UApplication *getApplication();

	void addWindow(UWindow *window);
	bool removeWindow(int id);
	bool removeWindow(UWindow *window);

	void setStartupWindow(int id);

	bool isContain(int id);
	UWindow* getWindowByPos(std::size_t pos);
	UWindow* getWindowById(int id);
	std::size_t getWindowCount();
	int getStartupWindow();
};