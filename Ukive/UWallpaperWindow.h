#pragma once
#include "UWindow.h"
#include "UAnimator.h"

#define ROTATE_CIRCLE 0
#define VANISH_DOT 1
#define ALPHA_TEXT 2
#define ENABLE_ALPHA_TEXT 3
#define EXTEND_DASHED 4

#define RADIUS_INNER 10
#define RADIUS_OUTTER 11

class UCanvas;

class UWallpaperWindow :
	public UWindow, public UAnimator::OnValueChangedListener
{
private:
	ID2D1Effect *mEffect;
	ID2D1SolidColorBrush *mCircleBrush;
	ID2D1RadialGradientBrush *mRadialBrush;

	float mInitTextWidth;
	float mThreeDotWidth;
	float mStandbyTextWidth;
	std::wstring mInitText;
	std::wstring mStandbyText;

	std::wstring mBottomText;
	IDWriteTextFormat* mBottomTextFormat;

	UAnimator *mAnimator;

	void createRadialGradientBrush();
	float getTextWidth(std::wstring text, IDWriteTextFormat* format);
	
	void drawInnerArc(UCanvas *c, float cx, float cy, float radius);
	void drawDecoredCircle(
		UCanvas *c, float cx, float cy, float radius,
		float extAngle, float extRadiusAdd, float strokeWidth, D2D1_COLOR_F &color);
	void drawDashedLines(UCanvas *c, float cx, float cy, float radius);

	void drawCenterCircle(UCanvas *canvas, float radiusMinus);
	void drawDeformedCircle(
		UCanvas *canvas, float radiusMinus, bool showOutline, float outlineWidth);
	void drawCursor(UCanvas *canvas, bool split, float distance);
	void drawText(UCanvas *canvas, std::wstring text, float alpha);

public:
	using UWindow::UWindow;

	void onCreate() override;
	void onDraw(UCanvas *canvas) override;
	void onDestroy() override;
	void onInputEvent(UInputEvent *e) override;

	void onValueChanged(
		unsigned int varIndex,
		IUIAnimationStoryboard *storyboard,
		IUIAnimationVariable *variable,
		double newValue, double previousValue) override;
	void onIntegerValueChanged(
		unsigned int varIndex,
		IUIAnimationStoryboard *storyboard,
		IUIAnimationVariable *variable,
		int newValue, int previousValue) override;
};