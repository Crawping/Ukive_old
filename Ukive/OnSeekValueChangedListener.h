#pragma once

class SeekBar;

class OnSeekValueChangedListener
{
public:
	virtual void onSeekValueChanged(SeekBar *seekBar, float value) = 0;
	virtual void onSeekIntegerValueChanged(SeekBar *seekBar, int value) = 0;
};