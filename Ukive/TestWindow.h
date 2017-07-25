#pragma once
#include "UWindow.h"


class TestWindow : public UWindow
{
public:
	TestWindow(UApplication *app);
	TestWindow(UApplication *app, int id);
	~TestWindow();

	virtual void onCreate() override;
};