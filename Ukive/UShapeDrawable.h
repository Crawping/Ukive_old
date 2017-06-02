#pragma once
#include "UDrawable.h"

class UShapeDrawable : public UDrawable
{
private:
	int mWidth;
	int mHeight;
	int mShape;

	bool mHasSolid;
	bool mHasStroke;
	float mRoundRadius;
	float mStrokeWidth;
	D2D1_COLOR_F mSolidColor;
	D2D1_COLOR_F mStrokeColor;

public:
	static const int SHAPE_RECT = 0;
	static const int SHAPE_ROUND_RECT = 1;
	static const int SHAPE_OVAL = 2;
	
public:
	UShapeDrawable(UWindow *wnd, int shape);
	~UShapeDrawable();

	void setSize(int width, int height);
	void setRadius(float radius);

	void setSolidEnable(bool enable);
	void setSolidColor(D2D1_COLOR_F color);
	void setStrokeEnable(bool enable);
	void setStrokeWidth(float width);
	void setStrokeColor(D2D1_COLOR_F color);

	virtual void draw(UCanvas *canvas);

	virtual float getOpacity();

	virtual int getIncWidth();
	virtual int getIncHeight();
};