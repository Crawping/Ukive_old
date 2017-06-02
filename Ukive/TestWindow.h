#pragma once
#include "UWindow.h"


class TestWindow : public UWindow
{
public:
	using UWindow::UWindow;

	virtual void onCreate() override;
};