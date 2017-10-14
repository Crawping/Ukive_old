#pragma once
#include "View.h"
#include "UComPtr.h"

class UBitmapDrawable;

class ImageView : public View
{
private:
	UBitmapDrawable *mBitmapDrawable;

public:
	ImageView(UWindow *wnd);
	ImageView(UWindow *wnd, int id);
	~ImageView();

	virtual void onMeasure(int width, int height, int widthSpec, int heightSpec);
	virtual void onDraw(UCanvas *canvas);
	virtual bool onInputEvent(UInputEvent *e);

	virtual void onSizeChanged(int width, int height, int oldWidth, int oldHeight);

	void setImageBitmap(UComPtr<ID2D1Bitmap> bitmap);
};