#include "UCommon.h"
#include "UWindow.h"
#include "UMenuItemImpl.h"


UMenuItemImpl::UMenuItemImpl(UWindow *wnd, int menuId, std::int32_t order)
	:UTextView(wnd)
{
	mOrder = order;
	mMenuId = menuId;
	mIsVisible = true;
	initMenuItem();
}

UMenuItemImpl::UMenuItemImpl(UWindow *wnd, int id, int menuId, std::int32_t order)
	: UTextView(wnd, id)
{
	mOrder = order;
	mMenuId = menuId;
	mIsVisible = true;
	initMenuItem();
}

UMenuItemImpl::~UMenuItemImpl()
{
}


void UMenuItemImpl::initMenuItem()
{
	this->setTextSize(13);
	this->setIsEditable(false);
	this->setIsSelectable(false);
	this->setPadding(16, 0, 16, 0);
	this->setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	this->autoWrap(false);
}


void UMenuItemImpl::setItemTitle(std::wstring title)
{
	mTitle = title;
	this->setText(title);
}

void UMenuItemImpl::setItemVisible(bool visible)
{
	if (mIsVisible == visible) return;

	mIsVisible = visible;
	this->setVisibility(visible ? UWidget::VISIBLE : UWidget::VANISHED);
}

void UMenuItemImpl::setItemEnabled(bool enable)
{
	this->setEnabled(enable);
}


int UMenuItemImpl::getItemId()
{
	return mMenuId;
}

std::int32_t UMenuItemImpl::getItemOrder()
{
	return mOrder;
}

std::wstring UMenuItemImpl::getItemTitle()
{
	return this->getText();
}

bool UMenuItemImpl::isItemVisible()
{
	return mIsVisible;
}

bool UMenuItemImpl::isItemEnabled()
{
	return this->isEnabled();
}