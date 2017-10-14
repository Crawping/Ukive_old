#pragma once
#include "FrameLayout.h"
#include "UMenu.h"

class UWindow;
class LayoutParams;
class LinearLayout;

class BaseLayout : public FrameLayout
{
private:
	std::shared_ptr<FrameLayout> mShadeLayout;
	std::shared_ptr<LinearLayout> mContentLayout;

	void initBaseLayout();

protected:
	virtual LayoutParams *generateLayoutParams(LayoutParams *lp) override;
	virtual LayoutParams *generateDefaultLayoutParams() override;
	virtual bool checkLayoutParams(LayoutParams *lp) override;

public:
	BaseLayout(UWindow *wnd);
	BaseLayout(UWindow *wnd, int id);
	~BaseLayout();

	void addShade(View *shade);
	void removeShade(View *shade);

	void addContent(View *content);

	virtual View *findWidgetById(int id) override;
};