#pragma once
#include "UComPtr.h"

class UAnimationManager
{
private:
	UComPtr<IUIAnimationManager> mAnimationManager;
	UComPtr<IUIAnimationTimer> mAnimationTimer;

public:
	static IUIAnimationTransitionLibrary *sTransitionLibrary;
	static IUIAnimationTransitionFactory *sTransitionFactory;

	class OnStateChangedListener
	{
	public:
		virtual void onStateChanged(
			UI_ANIMATION_MANAGER_STATUS newStatus,
			UI_ANIMATION_MANAGER_STATUS previousStatus) = 0;
	};

	class OnTimerEventListener
	{
	public:
		virtual void OnPreUpdate() = 0;
		virtual void OnPostUpdate() = 0;
		virtual void OnRenderingTooSlow(unsigned int fps) = 0;
	};

public:
	UAnimationManager();
	~UAnimationManager();

	HRESULT init();
	void close();

	static HRESULT initGlobal();
	static void closeGlobal();

	void pause();
	void resume();
	void finish(double second);
	void abandon();
	bool isBusy();
	bool update();

	void connectTimer(bool enable);

	void setTimerEventListener(OnTimerEventListener *l);
	void setOnStateChangedListener(OnStateChangedListener *l);

	IUIAnimationManager *getAnimationManager();
	IUIAnimationTimer *getAnimationTimer();
	IUIAnimationTransitionLibrary *getTransitionLibrary();
};