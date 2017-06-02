#pragma once

class UWidget;

class OnClickListener
{
public:
	virtual void onClick(UWidget *widget) = 0;
};