#include "UCommon.h"
#include "UMath.h"
#include "UWindow.h"
#include "ULayoutParams.h"
#include "UBaseLayoutParams.h"
#include "UMenuImpl.h"
#include "UInputEvent.h"
#include "UInnerWindow.h"
#include "UFrameLayout.h"
#include "ULinearLayout.h"
#include "ULinearLayoutParams.h"
#include "UBaseLayout.h"


UBaseLayout::UBaseLayout(UWindow *wnd)
	:UFrameLayout(wnd)
{
	initBaseLayout();
}

UBaseLayout::UBaseLayout(UWindow *wnd, int id)
	: UFrameLayout(wnd, id)
{
	initBaseLayout();
}


UBaseLayout::~UBaseLayout()
{
}


void UBaseLayout::initBaseLayout()
{
	mContentLayout = std::make_shared<ULinearLayout>(getWindow());
	mContentLayout->setOrientation(ULinearLayout::VERTICAL);
	mContentLayout->setLayoutParams(
		new ULinearLayoutParams(
			ULinearLayoutParams::MATCH_PARENT,
			ULinearLayoutParams::MATCH_PARENT));
	addWidget(mContentLayout);

	mShadeLayout = std::make_shared<UFrameLayout>(getWindow());
	mShadeLayout->setCanConsumeMouseEvent(false);
	mShadeLayout->setLayoutParams(
		new ULayoutParams(
			ULayoutParams::MATCH_PARENT,
			ULayoutParams::MATCH_PARENT));
}


ULayoutParams *UBaseLayout::generateLayoutParams(
	ULayoutParams *lp)
{
	return new UBaseLayoutParams(lp);
}

ULayoutParams *UBaseLayout::generateDefaultLayoutParams()
{
	return new UBaseLayoutParams(
		UBaseLayoutParams::FIT_CONTENT,
		UBaseLayoutParams::FIT_CONTENT);
}

bool UBaseLayout::checkLayoutParams(ULayoutParams *lp)
{
	return typeid(*lp) == typeid(UBaseLayoutParams);
}


void UBaseLayout::addShade(UWidget *shade)
{
	mShadeLayout->addWidget(shade);
	if (mShadeLayout->getChildCount() == 1)
		addWidget(mShadeLayout);
}

void UBaseLayout::removeShade(UWidget *shade)
{
	mShadeLayout->removeWidget(shade);
	if (mShadeLayout->getChildCount() == 0)
		removeWidget(mShadeLayout);
}

void UBaseLayout::addContent(UWidget *content)
{
	mContentLayout->addWidget(content);
}


UWidget *UBaseLayout::findWidgetById(int id)
{
	return mContentLayout->findWidgetById(id);
}