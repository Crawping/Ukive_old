#pragma once
#include "UWidget.h"
#include "UAnimator.h"
#include "OnSeekValueChangedListener.h"

class UWindow;

class USeekBar : public UWidget, public UAnimator::OnValueChangedListener
{
private:
	int mSeekTrackHeight;
	int mSeekThumbMinDiameter;
	int mSeekThumbMaxDiameter;
	float mSeekThumbCurDiameter;

	float mMaximum;
	float mSeekPercent;

	UAnimator *mThumbInAnimator;
	UAnimator *mThumbOutAnimator;
	OnSeekValueChangedListener *mListener;

	void initSeekBar();

private:
	bool mIsMouseLeftKeyAvailable;

	bool isMouseInThumb(int mouseX, int mouseY);
	bool isMouseInTrack(int mouseX, int mouseY);
	void computePercent(int mouseX, int mouseY);

	void startZoomInAnimation();
	void startZoomOutAnimation();

public:
	USeekBar(UWindow *window);
	USeekBar(UWindow *window, int id);
	~USeekBar();

	void setMaximum(float maximum);
	void setProgress(float progress, bool notify = false);
	float getProgress();

	void setOnSeekValueChangedListener(OnSeekValueChangedListener *l);

	virtual void onMeasure(
		int width, int height,
		int widthMode, int heightMode) override;

	virtual void onDraw(UCanvas *canvas) override;

	virtual bool onInputEvent(UInputEvent *e) override;

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