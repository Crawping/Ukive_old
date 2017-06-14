#include "UCommon.h"
#include "UMath.h"
#include "ULayoutParams.h"
#include "UColorDrawable.h"
#include "URippleDrawable.h"
#include "ULinearLayoutParams.h"
#include "UMenuItemImpl.h"
#include "UMenuImpl.h"


UMenuImpl::UMenuImpl(UWindow *wnd)
	:ULinearLayout(wnd)
{
	initMenu();
}

UMenuImpl::UMenuImpl(UWindow *wnd, int id)
	: ULinearLayout(wnd, id)
{
	initMenu();
}

UMenuImpl::~UMenuImpl()
{
}


void UMenuImpl::initMenu()
{
	mCallback = nullptr;
	mItemHeight = ULayoutParams::FIT_CONTENT;
}


void UMenuImpl::setMenuItemHeight(int height)
{
	mItemHeight = height;
}


void UMenuImpl::setCallback(UMenuCallback *callback)
{
	mCallback = callback;
}

UMenuCallback *UMenuImpl::getCallback()
{
	return mCallback;
}


UMenuItem *UMenuImpl::addItem(int id, std::int32_t order, std::wstring title)
{
	UMenuItem *item = new UMenuItemImpl(mWindow, id, order);
	item->setItemTitle(title);

	UWidget *widget = dynamic_cast<UWidget*>(item);
	widget->setBackground(new URippleDrawable(mWindow));
	widget->setOnClickListener(this);

	int insertedIndex = getChildCount();
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		UMenuItem *childItem = dynamic_cast<UMenuItem*>(child);
		if (childItem->getItemOrder() > order)
		{
			insertedIndex = i;
			break;
		}
	}
	
	this->addWidget(insertedIndex, widget, new ULinearLayoutParams(
		ULayoutParams::MATCH_PARENT, 
		mItemHeight));
	return item;
}

bool UMenuImpl::removeItem(int id)
{
	bool removed = false;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		UMenuItem *item = dynamic_cast<UMenuItem*>(child);
		if (item && item->getItemId() == id)
		{
			removeWidget(child);

			--i;
			removed = true;
		}
	}

	return removed;
}

bool UMenuImpl::hasItem(int id)
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		UMenuItem *item = dynamic_cast<UMenuItem*>(child);
		if (item && item->getItemId() == id)
			return true;
	}

	return false;
}

UMenuItem *UMenuImpl::findItem(int id)
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		UMenuItem *item = dynamic_cast<UMenuItem*>(child);
		if (item && item->getItemId() == id)
			return item;
	}

	return nullptr;
}

std::size_t UMenuImpl::getItemCount()
{
	return getChildCount();
}

void UMenuImpl::onClick(UWidget *widget)
{
	mCallback->onMenuItemClicked(this, dynamic_cast<UMenuItem*>(widget));
}