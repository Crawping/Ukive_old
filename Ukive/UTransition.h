#pragma once
#include "UComPtr.h"

class UTransition
{
private:
	UTransition();

	UComPtr<IUIAnimationTransition> mTransition;

public:
	~UTransition();

	enum UTSlope
	{
		INCREASING = 0,
		DECREASING = 1
	};

	UComPtr<IUIAnimationTransition> getTransition();

	static std::shared_ptr<UTransition> accDecTransition(
		double duration, double finalValue, 
		double accRatio, double decRatio);

	static std::shared_ptr<UTransition> constanceTransition(double duration);

	static std::shared_ptr<UTransition> cubicTransition(
		double duration, double finalValue, double finalVelocity);

	static std::shared_ptr<UTransition> discreteTransition(
		double delay, double finalValue, double hold);

	static std::shared_ptr<UTransition> instantaneousTransition(double finalValue);

	static std::shared_ptr<UTransition> linearTransition(double duration, double finalValue);

	static std::shared_ptr<UTransition> linearTransitionFromSpeed(double speed, double finalValue);

	static std::shared_ptr<UTransition> parabolicTransitionFromAcc(
		double finalValue, double finalVelocity, double acceleration);

	static std::shared_ptr<UTransition> reversalTransition(double duration);

	static std::shared_ptr<UTransition> sinusoidalTransitionFromRange(
		double duration, double minValue, double maxValue, 
		double period, UTSlope slope);

	static std::shared_ptr<UTransition> sinusoidalTransitionFromVelocity(
		double duration, double period);

	static std::shared_ptr<UTransition> smoothStopTransition(double maxDuration, double finalValue);

	static std::shared_ptr<UTransition> fromInterpolator(IUIAnimationInterpolator *interpolator);
};