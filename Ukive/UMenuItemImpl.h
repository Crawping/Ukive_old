#pragma once
#include "TextView.h"
#include "UMenuItem.h"

class UWindow;

class UMenuItemImpl : public TextView, public UMenuItem
{
private:
	int mMenuId;
	bool mIsVisible;
	std::wstring mTitle;
	std::int32_t mOrder;

	void initMenuItem();

public:
	UMenuItemImpl(UWindow *wnd, int menuId, std::int32_t order);
	UMenuItemImpl(UWindow *wnd, int id, int menuId, std::int32_t order);
	virtual ~UMenuItemImpl();

	void setItemTitle(std::wstring title) override;
	void setItemVisible(bool visible) override;
	void setItemEnabled(bool enable) override;

	int getItemId() override;
	std::int32_t getItemOrder() override;
	std::wstring getItemTitle() override;
	bool isItemVisible() override;
	bool isItemEnabled() override;
};