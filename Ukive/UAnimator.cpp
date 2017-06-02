#include "UCommon.h"
#include "UAnimationManager.h"
#include "UAnimationVariableChangeHandler.h"
#include "UAnimatorStateHandler.h"
#include "UAnimator.h"


UAnimator::UAnimator(UAnimationManager *mgr)
{
	mAnimationManager = mgr;
	mAnimatorStateListener = new UAnimatorStateHandler(this);

	HRESULT hr = mAnimationManager->getAnimationManager()
		->CreateStoryboard(&mStoryboard);
	if (FAILED(hr))
		throw std::runtime_error("UAnimator-Constructor(): create storyboard failed.");
	mStoryboard->SetStoryboardEventHandler(mAnimatorStateListener);
}


UAnimator::~UAnimator()
{
	stop();
	mAnimatorStateListener->Release();
}


void UAnimator::start()
{
	UI_ANIMATION_SECONDS time;
	mAnimationManager->getAnimationTimer()->GetTime(&time);
	mStoryboard->Schedule(time);
}

void UAnimator::stop()
{
	mStoryboard->Abandon();
}

void UAnimator::finish(double second)
{
	mStoryboard->Finish(second);
}

void UAnimator::reset()
{
	UI_ANIMATION_STORYBOARD_STATUS status;
	mStoryboard->GetStatus(&status);
	if (status == UI_ANIMATION_STORYBOARD_BUILDING)
		return;

	mVariableList.clear();

	mStoryboard->Abandon();
	mStoryboard.reset();
	HRESULT hr = mAnimationManager->getAnimationManager()
		->CreateStoryboard(&mStoryboard);
	if (FAILED(hr))
		throw std::runtime_error("UAnimator-Constructor(): create storyboard failed.");
	mStoryboard->SetStoryboardEventHandler(mAnimatorStateListener);
}

void UAnimator::startTransition(unsigned int varIndex, std::shared_ptr<UTransition> transition)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return;

	UI_ANIMATION_SECONDS time;
	mAnimationManager->getAnimationTimer()->GetTime(&time);

	mAnimationManager->getAnimationManager()->
		ScheduleTransition(it->second.get(), transition->getTransition().get(), time);
}


void UAnimator::setOnStateChangedListener(OnAnimatorListener *l)
{
	mAnimatorStateListener->setOnAnimatorListener(l);
}

void UAnimator::setOnValueChangedListener(
	unsigned int varIndex, OnValueChangedListener *l)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return;

	if (l == nullptr)
	{
		it->second->SetVariableChangeHandler(nullptr);
		it->second->SetVariableIntegerChangeHandler(nullptr);
	}
	else
	{
		UAnimationVariableChangeHandler *handler
			= new UAnimationVariableChangeHandler(l, varIndex);
		it->second->SetVariableChangeHandler(handler);
		it->second->SetVariableIntegerChangeHandler(handler);
		handler->Release();
	}
}


double UAnimator::getValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	double value = 0;
	it->second->GetValue(&value);

	return value;
}

int UAnimator::getIntValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	int value = 0;
	it->second->GetIntegerValue(&value);

	return value;
}

double UAnimator::getPrevValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	double value = 0;
	it->second->GetPreviousValue(&value);

	return value;
}

int UAnimator::getPrevIntValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	int value = 0;
	it->second->GetPreviousIntegerValue(&value);

	return value;
}

double UAnimator::getFinalValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	double value = 0;
	it->second->GetFinalValue(&value);

	return value;
}

int UAnimator::getFinalIntValue(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return 0;

	int value = 0;
	it->second->GetFinalIntegerValue(&value);

	return value;
}

double UAnimator::getElapsedTime()
{
	double time = 0;
	mStoryboard->GetElapsedTime(&time);
	return time;
}


bool UAnimator::addVariable(
	unsigned int varIndex, double initValue,
	double lower, double upper)
{
	auto it = mVariableList.find(varIndex);
	if (it != mVariableList.end())
		mVariableList.erase(varIndex);

	UComPtr<IUIAnimationVariable> aVariable;

	HRESULT hr = mAnimationManager->getAnimationManager()->
		CreateAnimationVariable(initValue, &aVariable);
	if (FAILED(hr))
		return false;

	aVariable->SetLowerBound(lower);
	aVariable->SetUpperBound(upper);
	
	mVariableList.insert(
		std::pair<unsigned int, UComPtr<IUIAnimationVariable>>(varIndex, aVariable));

	return true;
}

bool UAnimator::addTransition(unsigned int varIndex, std::shared_ptr<UTransition> transition)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return false;

	HRESULT hr = mStoryboard->AddTransition(
		it->second.get(), transition->getTransition().get());
	if (FAILED(hr))
		return false;

	return true;
}

bool UAnimator::addTransition(
	unsigned int varIndex, std::shared_ptr<UTransition> transition,
	UI_ANIMATION_KEYFRAME key)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return false;

	HRESULT hr = mStoryboard->AddTransitionAtKeyframe(
		it->second.get(), transition->getTransition().get(), key);
	if (FAILED(hr))
		return false;

	return true;
}

bool UAnimator::addTransition(
	unsigned int varIndex, std::shared_ptr<UTransition> transition,
	UI_ANIMATION_KEYFRAME startKey, UI_ANIMATION_KEYFRAME endKey)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return false;

	HRESULT hr = mStoryboard->AddTransitionBetweenKeyframes(
		it->second.get(), transition->getTransition().get(), startKey, endKey);
	if (FAILED(hr))
		return false;

	return true;
}

bool UAnimator::addKey(
	UI_ANIMATION_KEYFRAME existed, double offset, UI_ANIMATION_KEYFRAME *newKey)
{
	HRESULT hr = mStoryboard->AddKeyframeAtOffset(existed, offset, newKey);
	if (FAILED(hr))
		return false;

	return true;
}

bool UAnimator::addKey(
	std::shared_ptr<UTransition> transition, UI_ANIMATION_KEYFRAME *newKey)
{
	HRESULT hr = mStoryboard->AddKeyframeAfterTransition(transition->getTransition().get(), newKey);
	if (FAILED(hr))
		return false;

	return true;
}

bool UAnimator::hasVariable(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	return (it != mVariableList.end());
}

bool UAnimator::removeVariable(unsigned int varIndex)
{
	auto it = mVariableList.find(varIndex);
	if (it == mVariableList.end())
		return false;

	mVariableList.erase(varIndex);
	return true;
}