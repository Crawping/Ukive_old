#pragma once
#include "UWidget.h"
#include "UComPtr.h"

class UBitmapDrawable;

class UImageView : public UWidget
{
private:
	UBitmapDrawable *mBitmapDrawable;

public:
	UImageView(UWindow *wnd);
	UImageView(UWindow *wnd, int id);
	~UImageView();

	virtual void onMeasure(int width, int height, int widthSpec, int heightSpec);
	virtual void onDraw(UCanvas *canvas);
	virtual bool onInputEvent(UInputEvent *e);

	virtual void onSizeChanged(int width, int height, int oldWidth, int oldHeight);

	void setImageBitmap(UComPtr<ID2D1Bitmap> bitmap);
};