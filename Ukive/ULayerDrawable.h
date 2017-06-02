#pragma once
#include "UDrawable.h"

class UWindow;
class UCanvas;

class ULayerDrawable : public UDrawable
{
protected:
	std::vector<std::shared_ptr<UDrawable>> mDrawableList;

	void onBoundChanged(D2D1_RECT_F &newBound) override;

public:
	ULayerDrawable(UWindow *wnd);
	~ULayerDrawable();

	void addDrawable(UDrawable *drawable);
	void removeDrawable(UDrawable *drawable);

	virtual void draw(UCanvas *canvas);

	virtual float getOpacity();
};