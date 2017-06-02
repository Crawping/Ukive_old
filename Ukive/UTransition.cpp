#include "UCommon.h"
#include "UAnimationManager.h"
#include "UTransition.h"


UTransition::UTransition()
{
}

UTransition::~UTransition()
{
}


UComPtr<IUIAnimationTransition> UTransition::getTransition()
{
	return mTransition;
}


std::shared_ptr<UTransition> UTransition::accDecTransition(
	double duration, double finalValue,
	double accRatio, double decRatio)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateAccelerateDecelerateTransition(
			duration, finalValue, accRatio, decRatio, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::constanceTransition(double duration)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateConstantTransition(duration, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::cubicTransition(
	double duration, double finalValue, double finalVelocity)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateCubicTransition(duration, finalValue, finalVelocity, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::discreteTransition(
	double delay, double finalValue, double hold)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateDiscreteTransition(delay, finalValue, hold, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::instantaneousTransition(double finalValue)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateInstantaneousTransition(finalValue, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::linearTransition(double duration, double finalValue)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateLinearTransition(duration, finalValue, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::linearTransitionFromSpeed(double speed, double finalValue)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateLinearTransitionFromSpeed(speed, finalValue, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::parabolicTransitionFromAcc(
	double finalValue, double finalVelocity, double acceleration)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateParabolicTransitionFromAcceleration(
			finalValue, finalVelocity, acceleration, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::reversalTransition(double duration)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateReversalTransition(duration, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::sinusoidalTransitionFromRange(
	double duration, double minValue, double maxValue,
	double period, UTSlope slope)
{
	UComPtr<IUIAnimationTransition> aTransition;

	UI_ANIMATION_SLOPE aSlop;
	if (slope == INCREASING)
		aSlop = UI_ANIMATION_SLOPE_INCREASING;
	else
		aSlop = UI_ANIMATION_SLOPE_DECREASING;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateSinusoidalTransitionFromRange(
			duration, minValue, maxValue, period, aSlop, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::sinusoidalTransitionFromVelocity(
	double duration, double period)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateSinusoidalTransitionFromVelocity(duration, period, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}

std::shared_ptr<UTransition> UTransition::smoothStopTransition(double maxDuration, double finalValue)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionLibrary->
		CreateSmoothStopTransition(maxDuration, finalValue, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}


std::shared_ptr<UTransition> UTransition::fromInterpolator(IUIAnimationInterpolator *interpolator)
{
	UComPtr<IUIAnimationTransition> aTransition;

	HRESULT hr = UAnimationManager::sTransitionFactory->
		CreateTransition(interpolator, &aTransition);
	if (SUCCEEDED(hr))
	{
		std::shared_ptr<UTransition> transition(new UTransition());
		transition->mTransition = aTransition;
		return transition;
	}

	return 0;
}