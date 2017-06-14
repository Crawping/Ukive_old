#pragma once
#include "UFrameLayout.h"
#include "UMenu.h"

class UWindow;
class ULayoutParams;
class ULinearLayout;

class UBaseLayout : public UFrameLayout
{
private:
	std::shared_ptr<UFrameLayout> mShadeLayout;
	std::shared_ptr<ULinearLayout> mContentLayout;

	void initBaseLayout();

protected:
	virtual ULayoutParams *generateLayoutParams(ULayoutParams *lp) override;
	virtual ULayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(ULayoutParams *lp) override;

public:
	UBaseLayout(UWindow *wnd);
	UBaseLayout(UWindow *wnd, int id);
	~UBaseLayout();

	void addShade(UWidget *shade);
	void removeShade(UWidget *shade);

	void addContent(UWidget *content);

	virtual UWidget *findWidgetById(int id) override;
};