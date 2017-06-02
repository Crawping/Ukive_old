#pragma once

class USeekBar;

class OnSeekValueChangedListener
{
public:
	virtual void onSeekValueChanged(USeekBar *seekBar, float value) = 0;
	virtual void onSeekIntegerValueChanged(USeekBar *seekBar, int value) = 0;
};