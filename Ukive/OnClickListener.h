#pragma once

class View;

class OnClickListener
{
public:
	virtual void onClick(View *widget) = 0;
};