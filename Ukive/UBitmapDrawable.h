#pragma once
#include "UComPtr.h"
#include "UDrawable.h"

class UBitmapDrawable : public UDrawable
{
private:
	float mOpacity;
	UComPtr<ID2D1Bitmap> mBitmap;

public:
	UBitmapDrawable(UWindow *wnd, UComPtr<ID2D1Bitmap> bitmap);
	~UBitmapDrawable();

	void setOpacity(float opt);

	virtual void draw(UCanvas *canvas) override;

	virtual float getOpacity() override;

	virtual int getIncWidth() override;
	virtual int getIncHeight() override;

	UComPtr<ID2D1Bitmap> getBitmap();
};