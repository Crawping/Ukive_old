#include "UCommon.h"
#include "UCanvas.h"
#include "ULayerDrawable.h"


ULayerDrawable::ULayerDrawable(UWindow *wnd)
	:UDrawable(wnd)
{
}

ULayerDrawable::~ULayerDrawable()
{
}


void ULayerDrawable::addDrawable(UDrawable *drawable)
{
	mDrawableList.push_back(std::shared_ptr<UDrawable>(drawable));
}

void ULayerDrawable::removeDrawable(UDrawable *drawable)
{
	for (auto it = mDrawableList.begin();
		it != mDrawableList.end(); ++it)
	{
		if ((*it).get() == drawable)
		{
			mDrawableList.erase(it);
			return;
		}
	}
}


void ULayerDrawable::onBoundChanged(D2D1_RECT_F &newBound)
{
	for (auto it = mDrawableList.begin();
		it != mDrawableList.end(); ++it)
	{
		(*it)->setBound(newBound);
	}
}


void ULayerDrawable::draw(UCanvas *canvas)
{
	for (auto it = mDrawableList.begin();
		it != mDrawableList.end(); ++it)
	{
		(*it)->draw(canvas);
	}
}

float ULayerDrawable::getOpacity()
{
	if (mDrawableList.empty())
		return 0.f;
	else
		return 1.f;
}